#include "hsisimulation.h"

#include <QDomElement>
#include <QHash>
#include "hsi.h"
#include "math.h"
#include "project.h"
#include "projectinput.h"
#include "projectinputraster.h"
#include "projectinputcsv.h"
#include "projectinputvector.h"
#include "rastermanager_interface.h"
#include "gdal_priv.h"


namespace HabitatModel{


HSISimulation::HSISimulation(QDomElement *elSimulation)
    : Simulation(elSimulation)
{
    m_HasRasters = false;

    QDomElement * elHSI = NULL;
    // Now Create our HSI object if there is one.
    int nSimulationHSIID = elSimulation->firstChildElement("HSIID").text().toInt();

    QDomNodeList elConfigHSIs = Project::GetConfigDom()->elementsByTagName("HSI");

    bool bHSIFound = false;
    for(int n= 0; n < elConfigHSIs.length(); n++){
        if (!bHSIFound){
            elHSI = new QDomElement(elConfigHSIs.at(n).toElement());
            int nTestHSIID = elHSI->firstChildElement("HSIID").text().toInt();
            if (nSimulationHSIID == nTestHSIID)
                bHSIFound = true;
        }
    }

    if (elHSI == NULL)
        Project::ProjectError(SEVERITY_ERROR, "Project is missing an HSI.");

    m_hsiRef = new HSI(elHSI);

    delete elHSI;

    QString sRawHSISourcePath = QDir::fromNativeSeparators(elSimulation->firstChildElement("HSISourcePath").text());
    m_HSISourcePath = Project::GetProjectRootPath()->filePath(Project::SanitizePath(sRawHSISourcePath));

    // Make a local copy of each data source as a local simulation object,
    // ready for preparation.
    LoadInputs();

    // Now, if this thing is a raster we need to add it to the ExtentRectangle
    // For this simulation
    AddRastersToExtents();

    // Now that all the inputs are loaded we know the extent of the laoded
    // Rasters and we can prepare the inputs.
    PrepareInputs();
}

void HSISimulation::AddRastersToExtents(){

    // No Rasters, no point
    if (!HasRasters())
        return;

    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);

    while (i.hasNext()) {
        i.next();
        // Here is the curve we want
        ProjectInput * pInput = i.value()->GetProjectInput();

        if ( dynamic_cast <ProjectInputRaster *> ( pInput )){

            Project::GetOutputXML()->Log("Adding Raster to extent: " + i.value()->GetProjectInput()->GetName() , 2);
            QString sRasterPath = pInput->GetSourceFilePath();
            const QByteArray QBRasterPath = sRasterPath.toLocal8Bit();
            RasterManager::RasterMeta * pRasterMeta = new RasterManager::RasterMeta(QBRasterPath.data());
            RasterUnion(pRasterMeta);
            delete pRasterMeta;
        }
    }
}

void HSISimulation::Run(){

    /**
     *
     *  Get and loop over all the simulationHSCInputs, comparing them to
     *  their curves along the way.
     *
     **/

    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);

    while (i.hasNext()) {
        i.next();
        SimulationHSCInput * pSimHSCInput= i.value();
        // Here is the curve we want
        HSC * pHSC = pSimHSCInput->GetHSICurve()->GetHSC();

        // Here is the corresponding input raster
        ProjectInput * pInput = i.value()->GetProjectInput();

        // Pure virtual function will decide if it's a categorical
        // or coordinate pair HSC
        pHSC->ProcessRaster( pInput->GetPreparedRasterFileName(),
                             pInput->GetHSOutputRasterFileName(),
                             GetRasterExtentMeta() );

    }

    /**
     *
     *  Combine Output Rasters using HSIMethodID in HSI
     *
     **/

    Project::GetOutputXML()->Log("Combining all output rasters into one: " + GetHSISourcePath() , 2);

    //Method of combination
    int nMethod = DetermineMethod();

    // Our final output Raster file name and path:
    QString sHSIOutputFile = GetHSISourcePath();
    Project::EnsureFile(sHSIOutputFile);

    static QHash<int, GDALDataset *> dDatasets;
    QHash<int, char *> dInBuffers;

    int sRasterCols = GetRasterExtentMeta()->GetCols();

    // Open all the inputs into a hash of datasets. We must remember to clean this up later
    i.toFront();
    while (i.hasNext()) {
        i.next();

        // Here is the corresponding input raster, added as a hash to a dataset
        ProjectInput * pSimHSCHSOutput = i.value()->GetProjectInput();
        const QByteArray sHSIOutputQB = pSimHSCHSOutput->GetHSOutputRasterFileName().toLocal8Bit();
        GDALDataset * pInputDS = (GDALDataset*) GDALOpen( sHSIOutputQB.data(), GA_ReadOnly);

        // Add a buffer for reading this input
        char * pReadBuffer = (char*) CPLMalloc(sizeof(double)*sRasterCols);

        // Notice these get the same keys.
        dDatasets.insert(i.key(), pInputDS);
        dInBuffers.insert(i.key(), pReadBuffer);
    }

    double dNoDataVal = GetRasterExtentMeta()->GetNoDataValue();

    // Step it down to char* for Rasterman and create+open an output file
    const QByteArray sHSIOutputQB = GetHSISourcePath().toLocal8Bit();
    GDALDataset * pOutputDS = RasterManager::CreateOutputDS( sHSIOutputQB.data(), GetRasterExtentMeta());
    double * pReadBuffer = (double*) CPLMalloc(sizeof(double) * sRasterCols);

    //loop through each DEM cell and do the hillshade calculation, do not loop through edge cells
    for (int i=0; i < GetRasterExtentMeta()->GetRows(); i++)
    {
        // Populate the buffers with a new line from each file.
        QHashIterator<int, GDALDataset *> QHDSIterator(dDatasets);
        while (QHDSIterator.hasNext()) {
            QHDSIterator.next();
            // Read the row
            QHDSIterator.value()->GetRasterBand(1)->RasterIO(GF_Read, 0,  i,
                                               sRasterCols, 1,
                                               dInBuffers.value(QHDSIterator.key()),
                                               sRasterCols, 1,
                                               GDT_Float64, 0, 0);
        }

        for (int j=0; j < sRasterCols; j++)
        {
            QHash<int, double> dCellContents;
            QHashIterator<int, char *> QHIBIterator(dInBuffers);
            while (QHIBIterator.hasNext()) {
                QHIBIterator.next();
                dCellContents.insert(QHIBIterator.key(), QHIBIterator.value()[j]);
            }

            switch (nMethod) {
            case HSI_PRODUCT:
                pReadBuffer[j] = HSICombineProduct(dCellContents, dNoDataVal);
                break;
            case HSI_ARITHMETIC_MEAN:
                pReadBuffer[j] = HSIArithmeticMean(dCellContents, dNoDataVal);
                break;
            case HSI_GEOMETRIC_MEAN:
                pReadBuffer[j] = HSIGeometricMean(dCellContents, dNoDataVal);
                break;
            case HSI_MINIMUM:
                pReadBuffer[j] = HSIMinimum(dCellContents, dNoDataVal);
                break;
            case HSI_WEIGHTED_MEAN:
                pReadBuffer[j] = HSIWeightedMean(dCellContents, dNoDataVal);
                break;
            default:
                break;
            }

        }
        pOutputDS->GetRasterBand(1)->RasterIO(GF_Write,0,i,
                                              sRasterCols,1,
                                              pReadBuffer,
                                              sRasterCols,1,
                                              GetRasterExtentMeta()->GetGDALDataType(),
                                              0,0 );
    }
    if ( pOutputDS != NULL)
        GDALClose(pOutputDS);
    CPLFree(pReadBuffer);
    pReadBuffer = NULL;

    // Let's remember to clean up the inputs
    QHashIterator<int, GDALDataset *> qhds(dDatasets);
    while (qhds.hasNext()) {
        qhds.next();
        GDALClose(qhds.value());
    }
    dDatasets.clear();
    QHashIterator<int, char *> qhbuff(dInBuffers);
    while (qhbuff.hasNext()) {
        qhbuff.next();
        CPLFree(qhbuff.value());
    }
    dInBuffers.clear();
}

void HSISimulation::PrepareInputs(){

    Project::GetOutputXML()->Log("Preparing inputs for HSI Simulation: " + GetName() , 2);

    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);
    while (i.hasNext()) {
        i.next();
        i.value()->GetProjectInput()->Prepare(GetRasterExtentMeta());
    }
}

int HSISimulation::DetermineMethod(){
    if (m_hsiRef->GetMethod() == NULL)
        Project::ProjectError(SEVERITY_ERROR, "This HSI has no method.");

    QString sMethod = m_hsiRef->GetMethod()->GetName();

    if ( sMethod.compare("Product") == 0 ){ return HSI_PRODUCT; }
    else if ( sMethod.compare("Arithmetic Mean") == 0 ){ return HSI_ARITHMETIC_MEAN; }
    else if ( sMethod.compare("Geometric Mean") == 0 ){ return HSI_GEOMETRIC_MEAN; }
    else if ( sMethod.compare("Minimum") == 0 ){ return HSI_MINIMUM; }
    else if ( sMethod.compare("Weighted Mean") == 0 ){ return HSI_WEIGHTED_MEAN; }

    Project::ProjectError(SEVERITY_ERROR, "Could not determine Method for Raster combination in HSI Simulation");
    return -1;

}

void HSISimulation::Clean(){

}

// This function loads the HSC Inputs but it also creates copies of the
// Inputs for preparation down in PrepareInputs()
void HSISimulation::LoadInputs(){

    Project::GetOutputXML()->Log("Loading Inputs for HSI Simulation: " + GetName() , 2);

    QDomNodeList elHSCInputs = Project::GetConfigDom()->elementsByTagName("SimulationHSCInputs");

    for(int n= 0; n < elHSCInputs.length(); n++){
        QDomElement elHSCInput = elHSCInputs.at(n).toElement();

        // It's easier to look up the hsi curve here.
        int nHSICurveID = elHSCInput.firstChildElement("HSICurveID").text().toInt();
        HSICurve * pHSICurve = m_hsiRef->GetCurve(nHSICurveID);

        SimulationHSCInput * newHSCInput = new SimulationHSCInput(elHSCInput, pHSICurve);

        ProjectInput * pInput = newHSCInput->GetProjectInput();
        if ( dynamic_cast <ProjectInputRaster *> ( pInput ))
            m_HasRasters = true;

        m_simulation_hsc_inputs.insert(n, newHSCInput);

    }
}

double HSISimulation::HSICombineProduct(QHash<int, double> dCellContents, double dNoDataVal){
    double dSum = 0;
    QHashIterator<int, double> x(dCellContents);
    while (x.hasNext()) {
        x.next();

        // If anything is NoData then that's the return
        if (x.value() == dNoDataVal)
            return dNoDataVal;

        dSum += x.value();
    }
    return dSum;
}

double HSISimulation::HSIArithmeticMean(QHash<int, double> dCellContents, double dNoDataVal){
    double dSum = 0;
    QHashIterator<int, double> x(dCellContents);
    while (x.hasNext()) {
        x.next();

        // If anything is NoData then that's the return
        if (x.value() == dNoDataVal)
            return dNoDataVal;

        dSum += x.value();
    }
    return dSum / dCellContents.size();
}

double HSISimulation::HSIGeometricMean(QHash<int, double> dCellContents, double dNoDataVal){
    double dProd = 0;
    QHashIterator<int, double> x(dCellContents);
    while (x.hasNext()) {
        x.next();

        // If anything is NoData then that's the return
        if (x.value() == dNoDataVal)
            return dNoDataVal;

        dProd *= x.value();
    }
    return pow(dProd, 1 / dCellContents.size() );
}

double HSISimulation::HSIMinimum(QHash<int, double> dCellContents, double dNoDataVal){
    double dMin = 0;
    bool first = true;
    QHashIterator<int, double> x(dCellContents);
    while (x.hasNext()) {
        x.next();

        // If anything is NoData then that's the return
        if (x.value() == dNoDataVal)
            return dNoDataVal;

        if (first){
            first = false;
            dMin = x.value();
        }
        else if(x.value() < dMin)
            dMin = x.value();
    }
    return dMin;
}

double HSISimulation::HSIWeightedMean(QHash<int, double> dCellContents, double dNoDataVal){
    Project::GetOutputXML()->LogDebug("WEIGHTED MEAN NOT YET IMPLEMENTED!", 3);
    QHashIterator<int, double> x(dCellContents);
    while (x.hasNext()) {
        return dNoDataVal;
        x.next();
    }
    return dNoDataVal;
}

HSISimulation::~HSISimulation(){
    // Empty HSC input store

    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);
    while (i.hasNext()) {
        i.next();
        delete i.value();
    }
    m_simulation_hsc_inputs.clear();

    delete m_hsiRef;

}


}
