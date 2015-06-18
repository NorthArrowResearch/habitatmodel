#include "fis.h"

namespace HabitatModel{


//<FIS>
//  <FISID>555</FISID>
//  <FISRuleFile>Simulations\SAMPLE_FIS\Outputs\SAMPLE_FIS.tif</FISRuleFile>
//</FIS>

FIS::FIS(QDomElement * elFIS, QHash<int, SimulationFISInput *> * pSimulationFisInputs, RasterManager::RasterMeta * RasterExtents) : NamedObjectWithID(elFIS, "FISName", "FISID")
{
    QString sFISRulePath = Project::SanitizePath(elFIS->firstChildElement("FISRuleFile").text());
    m_sFISRuleFile = Project::GetProjectRootPath()->filePath(sFISRulePath);

    if (!QFile(m_sFISRuleFile).exists())
        Project::ProjectError(FILE_NOT_FOUND, QString("Could not find the FIS Rule File: %1").arg(m_sFISRuleFile));

    m_pfis_inputs = pSimulationFisInputs;
    m_RasterExtents = RasterExtents;
}

FIS::~FIS()
{

}

void FIS::init(){

    rules = new FISRuleSet();
    // Load the FIS rule file
    rules->parseFile(m_sFISRuleFile);

    // Confirm that the number of inputs specified matches the number in the rule file
    if (rules->numInputs() != m_pfis_inputs->count())
        throw HabitatException(FIS_ERROR, "Number of FIS inputs must match the number in the rule file.");

}

/*
 * Opens datasets for reading and writing, allocates memory, and figures out if we need to check
 * for NODATA values in the input datasets. Most of the variables used in this method are class
 * members. The Datasources are set in the constructor. The Datasets opened here must be closed
 * in the finalizeProcess() method, especially the ones opened for writing (or data may not get
 * written properly). The arrays allocated here MUST be freed in the finalizeProcess() method
 * unless we want to introduce a memory leak.
 */
void FIS::RunRasterFis(QString sOutputFile)
{

    QHashIterator<int, SimulationFISInput *> dSimFISInputs( * m_pfis_inputs);
    QHash<int, GDALRasterBand *> dDatasets;
    QHash<int, double *> dInBuffers;
    std::vector<double> inputNoDataValues = std::vector<double>(m_pfis_inputs->count());
    std::vector<double*> inputData = std::vector<double*>(m_pfis_inputs->count());

    bool checkNoData = false;
    int sRasterCols = m_RasterExtents->GetCols();

    // Open all the inputs into a hash of datasets. We must remember to clean this up later

    if (rules->numInputs() != m_pfis_inputs->count())
        throw new HabitatException(FIS_ERROR, "Number of inputs does not match number of FIS inputs.");

    for (int i=0; i<rules->numInputs(); i++)
    {
        QString sInputname = QString::fromUtf8( rules->getInputName(i) );
        dSimFISInputs.toFront();
        bool found = false;
        while (dSimFISInputs.hasNext()) {
            dSimFISInputs.next();

            QString sFISInputName = dSimFISInputs.value()->GetFISInputName();
            if (sFISInputName.compare(sInputname, Qt::CaseInsensitive) == 0){
                found = true;
                // Here is the corresponding input raster, added as a hash to a dataset
                ProjectInput * pSimFISOutput = dSimFISInputs.value()->GetProjectInput();
                const QByteArray sHSIOutputQB = pSimFISOutput->GetPreparedRasterFileName().toLocal8Bit();
                GDALDataset * pInputDS = (GDALDataset*) GDALOpen( sHSIOutputQB.data(), GA_ReadOnly);
                GDALRasterBand * pInputRB = pInputDS->GetRasterBand(1);

                // Add a buffer for reading this input
                double * pReadBuffer = (double*) CPLMalloc(sizeof(double) * sRasterCols);

                // Notice these get the same keys.
                dDatasets.insert(dSimFISInputs.key(), pInputRB);
                dInBuffers.insert(dSimFISInputs.key(), pReadBuffer);

                int hasnodata = false;
                double nodataval = pInputRB->GetNoDataValue(&hasnodata);
                inputNoDataValues[dSimFISInputs.key()] = nodataval;
                if (nodataval > 0)
                    checkNoData = true;
            }

        }
        if (!found)
            throw new HabitatException(FIS_ERROR, QString( "Could not find FIS input named: %1").arg(sInputname) );
    }


    // Open the output dataset for writing and get its band.
    const QByteArray sFISOutputQB = sOutputFile.toLocal8Bit();
    GDALDataset * pOutputDS = RasterManager::CreateOutputDS( sFISOutputQB.data(), m_RasterExtents);
    GDALRasterBand * pOutputRB = pOutputDS->GetRasterBand(1);
    double * pReadBuffer = (double*) CPLMalloc(sizeof(double) * sRasterCols);

    // See if we need to worry about NODATA in the inputs.
    double * pOutputBuffer = (double*) CPLMalloc(sizeof(double) * sRasterCols);

    rules->initFuzzy();

    // Loop over the rows and columns. DO FIS!!
    for (int i=0; i < m_RasterExtents->GetRows(); i++)
    {

        // Populate the buffers with a new line from each file.
        QHashIterator<int, GDALRasterBand *> dDatasetIterator(dDatasets);
        while (dDatasetIterator.hasNext()) {
            dDatasetIterator.next();
            // Read the row
            dDatasetIterator.value()->RasterIO(GF_Read, 0,  i,
                                               sRasterCols, 1,
                                               dInBuffers.value(dDatasetIterator.key()),
                                               sRasterCols, 1,
                                               GDT_Float64,
                                               0, 0);
        }

        for (int j=0; j < sRasterCols; j++)
        {
            QHashIterator<int, GDALRasterBand *> dDatasetIterator(dDatasets);
            while (dDatasetIterator.hasNext()) {
                dDatasetIterator.next();
                inputData[dDatasetIterator.key()] = &dInBuffers.value(dDatasetIterator.key())[j];
            }

            pOutputBuffer[j] = rules->calculate(inputData, j, checkNoData,
                                                inputNoDataValues,
                                                m_RasterExtents->GetNoDataValue());
        }

        pOutputRB->RasterIO(GF_Write, 0, i,
                            sRasterCols, 1,
                            pOutputBuffer,
                            sRasterCols, 1,
                            GDT_Float64,
                            0, 0 );

    }

    if ( pOutputDS != NULL)
        GDALClose(pOutputDS);
    CPLFree(pReadBuffer);
    pReadBuffer = NULL;

    // Let's remember to clean up the inputs
    QHashIterator<int, GDALRasterBand *> qhds(dDatasets);
    while (qhds.hasNext()) {
        qhds.next();
        GDALClose(qhds.value());
    }
    dDatasets.clear();
    QHashIterator<int, double *> qhbuff(dInBuffers);
    while (qhbuff.hasNext()) {
        qhbuff.next();
        CPLFree(qhbuff.value());
    }
    dInBuffers.clear();

}


}
