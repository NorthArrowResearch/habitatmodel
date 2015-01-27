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
#include "rastermanager_exception.h"
#include "gdal_priv.h"
#include "simulation.h"
#include <QTextStream>
#include <QList>

namespace HabitatModel{

HSISimulation::HSISimulation(QDomElement *elSimulation)
    : Simulation(elSimulation)
{
    m_NumRasters = 0;
    m_NumCSVs = 0;

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

    // Make a local copy of each data source as a local simulation object,
    // ready for preparation.
    LoadInputs();

    // Now, if this thing is a raster we need to add it to the ExtentRectangle
    // For this simulation
    if (HasOutputRaster()){
        AddRastersToExtents();
    }
    // Now that all the inputs are loaded we know the extent of the laoded
    // Rasters and we can prepare the inputs.
    PrepareInputs();
}

void HSISimulation::AddRastersToExtents(){

    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);

    while (i.hasNext()) {
        i.next();
        // Here is the curve we want
        ProjectInput * pInput = i.value()->GetProjectInput();

        if ( dynamic_cast <ProjectInputRaster *> ( pInput )){
            try {
                Project::GetOutputXML()->Log("Adding Raster to extent: " + i.value()->GetProjectInput()->GetName() , 2);
                QString sRasterPath = pInput->GetSourceFilePath();
                const QByteArray QBRasterPath = sRasterPath.toLocal8Bit();

                RasterManager::RasterMeta * pRasterMeta = new RasterManager::RasterMeta(QBRasterPath.data());
                RasterUnion(pRasterMeta);
                delete pRasterMeta;
            }
            catch (RasterManager::RasterManagerException e){
                Project::GetOutputXML()->Log("ERROR:" + e.GetReturnMsgAsString() , 0);
            }

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
    Project::GetOutputXML()->Log("Starting Simulation Run: " + GetName() , 0);

    //Method of combination
    int nMethod = DetermineMethod();

    // This will work for Raster  and CSV+Raster output cases
    if (HasOutputRaster()){
        RunRasterHSI(nMethod);
    }

    // The above will work for both Raster or CSV+Raster but not for CSV only.
    if (HasOutputCSV() && !HasOutputRaster()){
        RunCSVHSI(nMethod);
    }

    Project::GetOutputXML()->AddResult(this, "WeightedUsableArea",  QString::number(m_dWeightedUse) );
    Project::GetOutputXML()->AddResult(this, "NormalizedWeightedUsableArea",  QString::number(m_dNormWeightedUse) );
    Project::GetOutputXML()->AddResult(this, "PercentOccupied",  QString::number(m_dPercentUsage) );
}

void HSISimulation::RunCSVHSI(int nmethod){
    Project::GetOutputXML()->Log("Beginning CSV input processing: " + m_bOutputCSV , 2);

    QString sXField;
    QString sYField;
    QString sInputCSVFile;
    QList<ProjectInputCSV * > qlInputList;
    QList<int> nKeepCols;
    QStringList qlKeepColNames;
    QHash<QString, double> qhInputValues;
    int nlinenumber = 0;

    // Find all the Inputs that come from a single CSV and collect them
    QHashIterator<int, SimulationHSCInput *> dSimHSCInputs(m_simulation_hsc_inputs);

    // Get all the CSV inputs and parse out the columns we need.
    while (dSimHSCInputs.hasNext()) {
        dSimHSCInputs.next();
        SimulationHSCInput * pSimHSCInput= dSimHSCInputs.value();
        // Here is the curve we want
        HSC * pHSC = pSimHSCInput->GetHSICurve()->GetHSC();

        // Here is the corresponding input raster
        ProjectInput * pInput = dSimHSCInputs.value()->GetProjectInput();
        if ( ProjectInputCSV * InputCSV = dynamic_cast <ProjectInputCSV *> ( pInput )){
            qlInputList.append(InputCSV);
            if (sXField.isEmpty()){
                sXField == InputCSV->GetXFieldName();
            }
            if (sYField.isEmpty()){
                sYField == InputCSV->GetYFieldName();
            }
            if (sInputCSVFile.isEmpty()){
                sInputCSVFile == InputCSV->GetInputFileName();
            }
        }
    }

    if (sXField.isEmpty()){
        Project::ProjectError(MISSING_FIELD, "the X field could not be determined.");
    }
    if (sInputCSVFile.isEmpty()){
        Project::ProjectError(MISSING_FIELD, "the CSV input path could not be determined.");
    }

    // Open the input CSV file as ReadOnly
    QFile InputCSVFile(m_bOutputCSV);
    if (!InputCSVFile.open(QFile::ReadOnly)){{
        throw HabitatException(FILE_NOT_FOUND, "Could not open CSV Input file for reading.");
    }

    // Create and open a new CSV file for writing
    QFile outputCSVFile(m_bOutputCSV);
    if (!outputCSVFile.open(QFile::WriteOnly|QFile::Truncate)){
        throw HabitatException(FILE_WRITE_ERROR, "Could not open file CSV output file for writing: " + m_bOutputCSV  );
    }
    QTextStream qtsOutputStream(outputCSVFile);

    // --------------------------------------------
    // Go line-by-line in the CSV file, calculating HSI values
    // --------------------------------------------
    int xcol=-1, ycol=-1, zcol=-1;

    while (line = InputCSVFile.readLine()) {
        QString line = file.readLine();
        QStringList slCSVcells = line.split(",");

        QListIterator<QString> nCSVCol(slCSVcells);

         // First line is the header and is where we decide what fields are what
        if (nlinenumber == 0){
            while (nCSVCol.hasNext()){
                nCSVCol.next();
                int idx = nCSVCol-slCSVcells.begin();
                slCSVcells.append("HSI Output");
                // Keep it if its the X Field
                if (csvItem.compare(sXField, Qt::CaseInsensitive) == 0 ){
                    xcol = idx;
                    nKeepCols.append(idx);
                }
                // Keep it if its the Y Field
                if (csvItem.compare(sYField, Qt::CaseInsensitive) == 0 ){
                    ycol = idx;
                    nKeepCols.append(idx);
                }
                // Keep it if its a data field. Also append a column on the header
                else if (LISTOFCOLNAMES.contains(csvItem)  ){
                    nKeepCols.append(idx);
                    slCSVcells.append("HS_" + csvItem);
                }
                // Throw away anything else
                else {
                    slCSVcells.removeAt(idx);
                }
            }
        }
        else {
            while (nCSVCol.hasNext()){
                nCSVCol.next();
                int idx = nCSVCol-slCSVcells.begin();

                QString csvItem = ProjectInputCSV::CSVCellClean(sCSVcell);
                else if (!nKeepCols.contains(idx)){
                    slCSVcells.removeAt(idx);
                }
            }

            // Now add the HSI value
            switch (nMethod) {
            case HSI_PRODUCT:
                slCSVcells.append(HSICombineProduct(dCellContents, dNoDataVal) );
                break;
            case HSI_ARITHMETIC_MEAN:
                slCSVcells.append( HSIArithmeticMean(dCellContents, dNoDataVal) );
                break;
            case HSI_GEOMETRIC_MEAN:
                slCSVcells.append( HSIGeometricMean(dCellContents, dNoDataVal) );
                break;
            case HSI_MINIMUM:
                slCSVcells.append( HSIMinimum(dCellContents, dNoDataVal) );
                break;
            case HSI_WEIGHTED_MEAN:
                slCSVcells.append( HSIWeightedMean(dCellContents, dNoDataVal) );
                break;
            default:
                break;
            }
        }

        // here's where we need to get the correct row of the output. Replace
        qtsOutputStream << slCSVcells.join(", ") << "\n"; // this writes first line with two columns
        nlinenumber++;
    }

    InputCSVFile.close();
    outputCSVFile.close();

}

void HSISimulation::RunRasterHSI(int nmethod){

    QHashIterator<int, SimulationHSCInput *> dSimHSCInputs(m_simulation_hsc_inputs);

    while (dSimHSCInputs.hasNext()) {
        dSimHSCInputs.next();
        SimulationHSCInput * pSimHSCInput= dSimHSCInputs.value();
        // Here is the curve we want
        HSC * pHSC = pSimHSCInput->GetHSICurve()->GetHSC();

        // Here is the corresponding input raster
        ProjectInput * pInput = dSimHSCInputs.value()->GetProjectInput();

        // Pure virtual function will decide if it's a categorical
        // or coordinate pair HSC
        try{
            pHSC->ProcessRaster( pInput->GetPreparedRasterFileName(),
                                 pInput->GetHSOutputRasterFileName(),
                                 GetRasterExtentMeta() );
        }
        catch (RasterManager::RasterManagerException e){
            throw HabitatException(RASTERMAN_EXCEPTION, e.GetReturnMsgAsString());
        }
    }

    /**
     *
     *  Combine Output Rasters using HSIMethodID in HSI
     *
     **/

    Project::GetOutputXML()->Log("Combining all output rasters into one: " + GetHSISourcePath() , 2);


    // Our final output Raster file name and path:
    Project::EnsureFile(m_bOutputRaster);

    QHash<int, GDALRasterBand *> dDatasets;
    QHash<int, double *> dInBuffers;

    //    if (HasRasters() == false) {
    //        throw HabitatException(NO_RASTERS, "For now if you want a raster output you need to have at least one raster in your inputs.");
    //    }
    int sRasterCols = GetRasterExtentMeta()->GetCols();

    // Open all the inputs into a hash of datasets. We must remember to clean this up later
    dSimHSCInputs.toFront();

    while (dSimHSCInputs.hasNext()) {
        dSimHSCInputs.next();

        // Here is the corresponding input raster, added as a hash to a dataset
        ProjectInput * pSimHSCHSOutput = dSimHSCInputs.value()->GetProjectInput();
        const QByteArray sHSIOutputQB = pSimHSCHSOutput->GetHSOutputRasterFileName().toLocal8Bit();
        GDALDataset * pInputDS = (GDALDataset*) GDALOpen( sHSIOutputQB.data(), GA_ReadOnly);
        GDALRasterBand * pInputRB = pInputDS->GetRasterBand(1);

        // Add a buffer for reading this input
        double * pReadBuffer = (double*) CPLMalloc(sizeof(double) * sRasterCols);

        // Notice these get the same keys.
        dDatasets.insert(dSimHSCInputs.key(), pInputRB);
        dInBuffers.insert(dSimHSCInputs.key(), pReadBuffer);
    }

    double dNoDataVal = GetRasterExtentMeta()->GetNoDataValue();

    // Step it down to char* for Rasterman and create+open an output file
    const QByteArray sHSIOutputQB = m_bOutputRaster.toLocal8Bit();
    GDALDataset * pOutputDS = RasterManager::CreateOutputDS( sHSIOutputQB.data(), GetRasterExtentMeta());
    GDALRasterBand * pOutputRB = pOutputDS->GetRasterBand(1);
    double * pReadBuffer = (double*) CPLMalloc(sizeof(double) * sRasterCols);

    double cellSum = 0;
    double usedCellCounter = 0;
    double cellArea = fabs( GetRasterExtentMeta()->GetCellHeight() * GetRasterExtentMeta()->GetCellWidth() );
    //loop through each DEM cell and do the hillshade calculation, do not loop through edge cells
    for (int i=0; i < GetRasterExtentMeta()->GetRows(); i++)
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
            QHash<int, double> dCellContents;
            QHashIterator<int, double *> QHIBIterator(dInBuffers);
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
            if (pReadBuffer[j] != dNoDataVal){
                cellSum += pReadBuffer[j];
                usedCellCounter++;
            }

        }
        pOutputRB->RasterIO(GF_Write, 0, i,
                            sRasterCols, 1,
                            pReadBuffer,
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

    // Now write some results
    m_dWeightedUse = cellSum * cellArea;
    m_dNormWeightedUse = m_dWeightedUse / usedCellCounter;
    m_dPercentUsage = 100 * usedCellCounter / ( GetRasterExtentMeta()->GetRows() * GetRasterExtentMeta()->GetCols() );


}

void HSISimulation::PrepareInputs(){

    Project::GetOutputXML()->Log("Preparing inputs for HSI Simulation: " + GetName() , 2);

    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);
    while (i.hasNext()) {
        i.next();
        Simulation * pThisSim = this;
        i.value()->GetProjectInput()->Prepare(pThisSim);
    }
}

int HSISimulation::DetermineMethod(){
    if (m_hsiRef->GetMethod() == NULL)
        Project::ProjectError(SEVERITY_ERROR, "This HSI has no method.");

    QString sMethod = m_hsiRef->GetMethod()->GetName();

    if ( sMethod.compare("Product", Qt::CaseInsensitive ) == 0 ){ return HSI_PRODUCT; }
    else if ( sMethod.compare("Arithmetic Mean", Qt::CaseInsensitive ) == 0 ){ return HSI_ARITHMETIC_MEAN; }
    else if ( sMethod.compare("Geometric Mean", Qt::CaseInsensitive ) == 0 ){ return HSI_GEOMETRIC_MEAN; }
    else if ( sMethod.compare("Minimum", Qt::CaseInsensitive) == 0 ){ return HSI_MINIMUM; }
    else if ( sMethod.compare("Weighted Mean", Qt::CaseInsensitive) == 0 ){ return HSI_WEIGHTED_MEAN; }

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

        // Test to see that our HSCInputs belong to this simulation.
        int nHSISimulationID = elHSCInput.firstChildElement("SimulationID").text().toInt();
        if (GetID() == nHSISimulationID){
            SimulationHSCInput * newHSCInput = new SimulationHSCInput(elHSCInput, pHSICurve);

            ProjectInput * pInput = newHSCInput->GetProjectInput();
            if ( dynamic_cast <ProjectInputRaster *> ( pInput ))
                m_HasRasters = true;

            m_simulation_hsc_inputs.insert(n, newHSCInput);
        }

    }
}

double HSISimulation::HSICombineProduct(QHash<int, double> dCellContents, double dNoDataVal){
    double dProd = 1;
    QHashIterator<int, double> x(dCellContents);
    while (x.hasNext()) {
        x.next();

        // If anything is NoData then that's the return
        if (x.value() == dNoDataVal)
            return dNoDataVal;

        dProd *= x.value();
    }
    return dProd;
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
    double dProd = 1;
    QHashIterator<int, double> x(dCellContents);
    while (x.hasNext()) {
        x.next();

        // If anything is NoData then that's the return
        if (x.value() == dNoDataVal)
            return dNoDataVal;

        dProd *= x.value();
    }
    return pow(dProd, (1 / (double) dCellContents.size()) );
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
    double dWMean = 0;
    QHashIterator<int, double> x(dCellContents);
    while (x.hasNext()) {
        x.next();
        // If anything is NoData then that's the return NoData
        if (x.value() == dNoDataVal)
            return dNoDataVal;

        HSICurve * dCurve = m_simulation_hsc_inputs.value(x.key())->GetHSICurve();
        dWMean += x.value() * dCurve->GetWeight();
    }
    return dWMean / dCellContents.size();
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
