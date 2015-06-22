#include "fis.h"
#include "benchmark.h"

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
    Project::GetOutputXML()->Log("Beginning Raster FIS Simulation run: " + sOutputFile , 2);

    QHashIterator<int, SimulationFISInput *> dSimFISInputs( * m_pfis_inputs);
    QHash<int, GDALRasterBand *> dDatasets;
    QHash<int, double *> dInBuffers;
    std::vector<double> inputNoDataValues = std::vector<double>(m_pfis_inputs->count());
    std::vector<double> inputData = std::vector<double>(m_pfis_inputs->count());

    bool checkNoData = m_RasterExtents->HasNoDataValue();
    double dNodataVal = m_RasterExtents->GetNoDataValue();

    int sRasterCols = m_RasterExtents->GetCols();

    // Open all the inputs into a hash of datasets. We must remember to clean this up later

    if (rules->numInputs() != m_pfis_inputs->count())
        throw new HabitatException(FIS_ERROR, "Number of inputs does not match number of FIS inputs.");

    Project::GetOutputXML()->Log(QString("Preparing %1 inputs for FIS.").arg(m_pfis_inputs->count()), 3 );
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
                dDatasets.insert(i, pInputRB);
                dInBuffers.insert(i, pReadBuffer);

                int hasnodata = false;
                double nodataval = pInputRB->GetNoDataValue(&hasnodata);
                inputNoDataValues[i] = nodataval;
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

    ProcessTimer FISTimer("Doing FIS");
    LoopTimer lineLoop("Process Line");  //DEBUG Only
    LoopTimer valLoop("Cells with Value");  //DEBUG Only
    LoopTimer nodataLoop("Cells with NoDATA");  //DEBUG Only

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
                int nDataKey = dDatasetIterator.key();
                inputData[nDataKey] = dInBuffers.value(nDataKey)[j];
            }

            pOutputBuffer[j] = rules->calculate(inputData, checkNoData,
                                                inputNoDataValues,
                                                dNodataVal);

            if (pOutputBuffer[j] == dNodataVal)
                valLoop.Tick();
            else
                nodataLoop.Tick();

        }

        pOutputRB->RasterIO(GF_Write, 0, i,
                            sRasterCols, 1,
                            pOutputBuffer,
                            sRasterCols, 1,
                            GDT_Float64,
                            0, 0 );
        lineLoop.Tick();

    }
    FISTimer.Output();

    lineLoop.Output(); // DEBUG only
    nodataLoop.Output(); // DEBUG only
    valLoop.Output(); //DEBUG only

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

void FIS::RunCSVFis(QString sOutputFile)
{
    Project::GetOutputXML()->Log("Beginning CSV input FIS Run: " + sOutputFile , 2);

    QString sXField, sYField, sInputCSVFile;
    QHashIterator<int, SimulationFISInput *> dSimFISInputs( * m_pfis_inputs);

    // The first in tis the rule file index. The second int is the csv column index
    QHash<int, int> qhFISInputOrder;
    double dNoDataVal = std::numeric_limits<double>::min();
    std::vector<double> inputNoDataValues = std::vector<double>(m_pfis_inputs->count());
    std::fill (inputNoDataValues.begin(),inputNoDataValues.end(),dNoDataVal);
    std::vector<double> inputData = std::vector<double>(m_pfis_inputs->count());

    Project::GetOutputXML()->Log(QString("Preparing %1 inputs for FIS.").arg(m_pfis_inputs->count()), 3 );

    dSimFISInputs.toFront();

    // We need the name of the CSV file and the X and Y field so we do one
    // Loop to find the first instance.
    while (dSimFISInputs.hasNext()) {
        dSimFISInputs.next();

        // Here is the corresponding input raster
        ProjectInput * pInput = dSimFISInputs.value()->GetProjectInput();
        if ( ProjectInputCSV * InputCSV = dynamic_cast <ProjectInputCSV *> ( pInput )){
            // Set X,Y and file field from the first input with valid values
            sXField = InputCSV->GetXFieldName();
            sYField = InputCSV->GetYFieldName();
            sInputCSVFile = InputCSV->GetSourceFilePath();
            break; // Not ideal
        }
    }

    if (sXField.isEmpty()){
        Project::ProjectError(CSV_INPUT_ERROR, "the X field could not be determined.");
    }
    if (sInputCSVFile.isEmpty()){
        Project::ProjectError(CSV_INPUT_ERROR, "the CSV input path could not be determined.");
    }


    // Open the input CSV file as ReadOnly
    // --------------------------------------------
    QFile InputCSVFile(sInputCSVFile);
    if (!InputCSVFile.open(QFile::ReadOnly)){
        throw HabitatException(FILE_NOT_FOUND, "Could not open CSV Input file for reading.");
    }

    // Our final output CSV file name and path:
    Project::EnsureFile(sOutputFile);

    // Create and open a new CSV file for writing
    // --------------------------------------------
    QFile outputCSVFile(sOutputFile);
    if (!outputCSVFile.open(QFile::WriteOnly|QFile::Truncate)){
        throw HabitatException(FILE_WRITE_ERROR, "Could not open file CSV output file for writing: " + sOutputFile  );
    }
    QTextStream qtsOutputStream(&outputCSVFile);


    // --------------------------------------------
    // Go line-by-line in the CSV file, calculating HSI values
    // --------------------------------------------
    int xcol=-1, ycol=-1;
    int nlinenumber = 0;

    double cellSum = 0;
    double usedCellCounter = 0;

    while ( !InputCSVFile.atEnd() ){
        QString CSVline = InputCSVFile.readLine();
        //        "X", "Y", "Depth", "Velocity";
        QStringList slCSVcells = CSVline.split(","); //Note: this will fail if headers items contain commans
        QStringList slCSVInputs;
        QStringList slCSVOutputs;


        // Line 1: This is a special case
        // this is where we decide what to keep and what to lose
        // --------------------------------------------------------

        int nColNumber = 0;
        if ( nlinenumber == 0 ){
            // First we have to decide which columns to keep
            foreach (QString sCSVCol, slCSVcells){

                ProjectInputCSV::CSVCellClean(sCSVCol);

                // Keep it if its the X Field
                if (sCSVCol.compare(sXField, Qt::CaseInsensitive) == 0 ){
                    xcol = nColNumber;
                    slCSVInputs.append(sCSVCol);
                }
                // Keep it if its the Y Field
                else if (sCSVCol.compare(sYField, Qt::CaseInsensitive) == 0 ){
                    ycol = nColNumber;
                    slCSVInputs.append(sCSVCol);
                }
                else {
                    // Find all the Inputs that come from a single CSV and collect them
                    dSimFISInputs.toFront();
                    while (dSimFISInputs.hasNext()) {
                        dSimFISInputs.next();

                        // Here is the corresponding input raster
                        ProjectInput * pInput = dSimFISInputs.value()->GetProjectInput();
                        ProjectInputCSV * InputCSV = dynamic_cast <ProjectInputCSV *> ( pInput );
                        if ( InputCSV && sCSVCol.compare(InputCSV->GetValueFieldName(), Qt::CaseInsensitive) == 0 ){

                            // add this input to our list
                            slCSVInputs.append(sCSVCol);
                            for (int i=0; i < rules->numInputs(); i++){
                                qhFISInputOrder.insert(i, nColNumber);
                            }

                        }
                    }
                }
                nColNumber++;
            }
            // This is the final, combined output column
            slCSVOutputs.append("\"FIS Output\"");
        }

        else{

            // Add X and Y first without altering them
            if (xcol >= 0){
                QString xVal = slCSVcells.at(xcol);
                ProjectInputCSV::CSVCellClean( xVal );
                slCSVInputs.append(xVal);
            }
            else
                Project::ProjectError(CSV_INPUT_ERROR, "Could not find X-col" + sXField + " in CSV file.");

            if (ycol >= 0){
                QString yVal = slCSVcells.at(ycol);
                ProjectInputCSV::CSVCellClean( yVal );
                slCSVInputs.append(yVal);
            }

            // Now loop through all cols and add the ones we need.
            for (int i=0; i < rules->numInputs(); i++){
                foreach (QString sCSVCol, slCSVcells){

                    int nColForRule = qhFISInputOrder.find(i).value();
                    if (nColForRule == nColNumber){
                        // Now add the Data input value
                        ProjectInputCSV::CSVCellClean( sCSVCol );
                        slCSVInputs.append(sCSVCol);
                        if (double dCSVItem = sCSVCol.toDouble()){

                            if (dCSVItem != dNoDataVal){
                                inputData[i] =  dCSVItem;
                            }
                            else{
                                slCSVOutputs.append(" ");
                            }
                        }
                        // Could not make a double. Just insert a blank space
                        else {
                            slCSVOutputs.append(" ");
                        }

                    }

                    nColNumber++;
                }
            }

            double dCombinedVal = rules->calculate(inputData, false,
                                                   inputNoDataValues,
                                                   dNoDataVal);

            if (dCombinedVal == dNoDataVal)
                slCSVOutputs.append(" ");
            else {
                cellSum += dCombinedVal;
                usedCellCounter++;
                slCSVOutputs.append( QString::number(  dCombinedVal ) );
            }
        }

        // here's where we need to get the correct row of the output. Replace
        qtsOutputStream << slCSVInputs.join(", ") << ", " << slCSVOutputs.join(", ")  << "\n"; // this writes first line with two columns
        nlinenumber++;
    }

    // Now write some results

    // Note: Percent usage is not a useful stat here so it is not written.
    InputCSVFile.close();
    outputCSVFile.close();
}



}
