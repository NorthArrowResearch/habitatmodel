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
#include "habitat_exception.h"
#include "gdal_priv.h"
#include "simulation.h"
#include <QTextStream>
#include <QList>

namespace HabitatModel{

HSISimulation::HSISimulation(QDomElement *elSimulation)
    : Simulation(elSimulation)
{

    QDomElement * elHSI = NULL;
    // Now Create our HSI object if there is one.
    int nSimulationHSIID = elSimulation->firstChildElement("HSIID").text().toInt();

    QDomNodeList elConfigHSIs = Project::GetDefDom()->elementsByTagName("HSI");

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
        SimulationError(SEVERITY_ERROR, "Project is missing an HSI.");

    m_hsiRef = new HSI(elHSI);

    delete elHSI;

    // Make a local copy of each data source as a local simulation object,
    // ready for preparation.
    LoadInputs();

    // Now that all the inputs are loaded we know the extent of the laoded
    // Rasters and we can prepare the inputs.
    QTime qtPrepTime;
    qtPrepTime.start();
    PrepareInputs();
    Project::GetOutputXML()->AddStatus(this->GetName(), STATUS_PREPARED, STATUSTYPE_SIMULATION, qtPrepTime.elapsed()/1000 );

}

/**
 * @brief FISSimulation::AddRastersToExtents DEPPRECATED FOR NOW. We're going to do this work in the
 * UI instead
 */
void HSISimulation::AddRastersToExtents(){

    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);

    while (i.hasNext()) {
        i.next();
        // Here is the curve we want
        ProjectInput * pInput = i.value()->GetProjectInput();

        if ( dynamic_cast <ProjectInputRaster *> ( pInput )){
            try {
                SimulationLog("Adding Raster to extent: " + i.value()->GetProjectInput()->GetName() , 2);
                QString sRasterPath = pInput->GetSourceFilePath();
                const QByteArray QBRasterPath = sRasterPath.toLocal8Bit();
            }
            catch (RasterManager::RasterManagerException e){
                SimulationLog("ERROR:" + e.GetReturnMsgAsString() , 0);
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
    QTime qtRunTime;
    qtRunTime.start();

    SimulationLog("Starting Simulation Run: " + GetName() , 0);

    //Method of combination
    int nMethod = DetermineMethod();

    // This will work for Raster  and CSV+Raster output cases
    if (HasOutputRaster()){
        RunRasterHSI(nMethod);
        if (m_dWeightedUse >= 0)
            Project::GetOutputXML()->AddResult(this, "WeightedUsableArea",  QString::number(m_dWeightedUse) );
        if (m_dNormWeightedUse >= 0)
            Project::GetOutputXML()->AddResult(this, "NormalizedWeightedUsableArea",  QString::number(m_dNormWeightedUse) );
        if (m_dPercentUsage >= 0)
            Project::GetOutputXML()->AddResult(this, "PercentOccupied",  QString::number(m_dPercentUsage) );
        if (m_nOccupiedCells >= 0)
            Project::GetOutputXML()->AddResult(this, "OccupiedCells",  QString::number(m_nOccupiedCells) );
        if (m_dCellArea >= 0)
            Project::GetOutputXML()->AddResult(this, "CellArea",  QString::number(m_dCellArea) );
    }

    // The above will work for both Raster or CSV+Raster but not for CSV only.
    if (HasOutputCSV() && !HasOutputRaster()){
        RunCSVHSI(nMethod);
        if (m_nOccupiedCells >= 0)
            Project::GetOutputXML()->AddResult(this, "OccupiedCells",  QString::number(m_nOccupiedCells) );
        if (m_nCSVLines >= 0)
            Project::GetOutputXML()->AddResult(this, "CSVLines",  QString::number(m_nCSVLines) );
    }



    Project::GetOutputXML()->AddStatus(this->GetName(), STATUS_COMPLETE, STATUSTYPE_SIMULATION , qtRunTime.elapsed()/1000);
    SimulationLog("Simulation Complete", 1);
}

void HSISimulation::RunCSVHSI(int nMethod){
    SimulationLog("Beginning CSV input processing: " + m_bOutputCSV , 2);

    QString sXField, sYField, sInputCSVFile;
    QHash<int, HSC * > qhHSCs;
    QHashIterator<int, SimulationHSCInput *> dSimHSCInputs(m_simulation_hsc_inputs);
    double dNoDataVal = std::numeric_limits<double>::min();

    // We need the name of the CSV file and the X and Y field so we do one
    // Loop to find the first instance.
    while (dSimHSCInputs.hasNext()) {
        dSimHSCInputs.next();

        // Here is the corresponding input raster
        ProjectInput * pInput = dSimHSCInputs.value()->GetProjectInput();
        if ( ProjectInputCSV * InputCSV = dynamic_cast <ProjectInputCSV *> ( pInput )){
            // Set X,Y and file field from the first input with valid values
            sXField = InputCSV->GetXFieldName();
            sYField = InputCSV->GetYFieldName();
            sInputCSVFile = InputCSV->GetSourceFilePath();
            break; // Not ideal
        }
    }


    if (sXField.isEmpty()){
        SimulationError(CSV_INPUT_ERROR, "the X field could not be determined.");
    }
    if (sInputCSVFile.isEmpty()){
        SimulationError(CSV_INPUT_ERROR, "the CSV input path could not be determined.");
    }

    // Open the input CSV file as ReadOnly
    // --------------------------------------------
    QFile InputCSVFile(sInputCSVFile);
    if (!InputCSVFile.open(QFile::ReadOnly)){
        throw HabitatException(FILE_NOT_FOUND, "Could not open CSV Input file for reading.");
    }

    // Our final output CSV file name and path:
    Project::EnsureFile(m_bOutputCSV);

    // Create and open a new CSV file for writing
    // --------------------------------------------
    QFile outputCSVFile(m_bOutputCSV);
    if (!outputCSVFile.open(QFile::WriteOnly|QFile::Truncate)){
        throw HabitatException(FILE_WRITE_ERROR, "Could not open file CSV output file for writing: " + m_bOutputCSV  );
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
                    dSimHSCInputs.toFront();
                    while (dSimHSCInputs.hasNext()) {
                        dSimHSCInputs.next();
                        SimulationHSCInput * pSimHSCInput= dSimHSCInputs.value();

                        // Here is the corresponding input raster
                        ProjectInput * pInput = dSimHSCInputs.value()->GetProjectInput();
                        ProjectInputCSV * InputCSV = dynamic_cast <ProjectInputCSV *> ( pInput );
                        if ( InputCSV && sCSVCol.compare(InputCSV->GetValueFieldName(), Qt::CaseInsensitive) == 0 ){
                            // add this input to our list
                            slCSVInputs.append(sCSVCol);
                            slCSVOutputs.append("\"HS_" + sCSVCol + "\"");
                            HSC * pHSC = pSimHSCInput->GetHSICurve()->GetHSC();
                            qhHSCs.insert(nColNumber, pHSC);
                        }
                    }
                }
                nColNumber++;
            }
            // This is the final, combined output column
            slCSVOutputs.append("\"HSI Output\"");
        }
        else{
            QHash<int, double> dCellContents;


            // Add X and Y first without altering them
            if (xcol >= 0){
                QString xVal = slCSVcells.at(xcol);
                ProjectInputCSV::CSVCellClean( xVal );
                slCSVInputs.append(xVal);
            }
            else
                SimulationError(CSV_INPUT_ERROR, "Could not find X-col" + sXField + " in CSV file.");

            if (ycol >= 0){
                QString yVal = slCSVcells.at(ycol);
                ProjectInputCSV::CSVCellClean( yVal );
                slCSVInputs.append(yVal);
            }

            // Now loop through all cols and add the ones we need.
            foreach (QString sCSVCol, slCSVcells){
                if (qhHSCs.contains(nColNumber)){
                    // Now add the HSI value
                    ProjectInputCSV::CSVCellClean( sCSVCol );
                    slCSVInputs.append(sCSVCol);
                    if (double dCSVItem = sCSVCol.toDouble()){
                        HSC * cellHSC = qhHSCs.find(nColNumber).value();
                        double dProcessedCSVItem = cellHSC->ProcessValue(dCSVItem, dNoDataVal);

                        if ( dProcessedCSVItem != dNoDataVal ){
                            dCellContents.insert( nColNumber, dProcessedCSVItem );
                            slCSVOutputs.append( QString::number(dProcessedCSVItem) );
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
            double dCombinedVal = CombineValues(nMethod, dCellContents, dNoDataVal);
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
    m_nOccupiedCells = usedCellCounter;
    m_nCSVLines = nlinenumber;


    // Note: Percent usage is not a useful stat here so it is not written.
    InputCSVFile.close();
    outputCSVFile.close();

}

void HSISimulation::RunRasterHSI(int nMethod){

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
                                 pInput->GetOutputRasterFileName(),
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
    SimulationLog("Combining all output rasters into one: " + m_bOutputRaster , 2);


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
        const QByteArray sHSIOutputQB = pSimHSCHSOutput->GetOutputRasterFileName().toLocal8Bit();
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
            pReadBuffer[j] = CombineValues(nMethod, dCellContents, dNoDataVal);

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

    GDALClose(pOutputDS);
    CPLFree(pReadBuffer);
    pReadBuffer = NULL;

    // Let's remember to clean up the inputs
    QHashIterator<int, GDALRasterBand *> qhds(dDatasets);
    while (qhds.hasNext()) {
        qhds.next();
        GDALClose(qhds.value()->GetDataset());
    }
    dDatasets.clear();
    QHashIterator<int, double *> qhbuff(dInBuffers);
    while (qhbuff.hasNext()) {
        qhbuff.next();
        CPLFree(qhbuff.value());
    }
    dInBuffers.clear();

    // Now write some results
    m_dCellArea = cellArea;
    m_dWeightedUse = cellSum * cellArea;
    m_dNormWeightedUse = m_dWeightedUse / (usedCellCounter * cellArea);
    m_dPercentUsage = 100 * usedCellCounter / ( GetRasterExtentMeta()->GetRows() * GetRasterExtentMeta()->GetCols() );
    m_nOccupiedCells = usedCellCounter;


}

double HSISimulation::CombineValues(int nMethod, QHash<int, double> dCellContents, double dNoDataVal){
    if (dCellContents.size() == 0)
        return dNoDataVal;

    switch (nMethod) {
    case HSI_PRODUCT:
        return HSICombineProduct(dCellContents, dNoDataVal);
        break;
    case HSI_ARITHMETIC_MEAN:
        return HSIArithmeticMean(dCellContents, dNoDataVal);
        break;
    case HSI_GEOMETRIC_MEAN:
        return HSIGeometricMean(dCellContents, dNoDataVal);
        break;
    case HSI_MINIMUM:
        return HSIMinimum(dCellContents, dNoDataVal);
        break;
    case HSI_WEIGHTED_MEAN:
        return  HSIWeightedMean(dCellContents, dNoDataVal);
        break;
    default:
        return dNoDataVal;
        break;
    }
}

void HSISimulation::PrepareInputs(){

    SimulationLog("Preparing inputs for HSI Simulation: " + GetName() , 2);

    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);
    while (i.hasNext()) {
        i.next();
        Simulation * pThisSim = this;
        // A purely CSV output request needs no preparing of outputs
        if (HasOutputRaster())
            i.value()->GetProjectInput()->Prepare(pThisSim);
    }
}

int HSISimulation::DetermineMethod(){
    if (m_hsiRef->GetMethod() == NULL)
        SimulationError(SEVERITY_ERROR, "This HSI has no method.");

    QString sMethod = m_hsiRef->GetMethod()->GetName();

    if ( sMethod.compare("Product", Qt::CaseInsensitive ) == 0 ){ return HSI_PRODUCT; }
    else if ( sMethod.compare("Arithmetic Mean", Qt::CaseInsensitive ) == 0 ){ return HSI_ARITHMETIC_MEAN; }
    else if ( sMethod.compare("Geometric Mean", Qt::CaseInsensitive ) == 0 ){ return HSI_GEOMETRIC_MEAN; }
    else if ( sMethod.compare("Minimum", Qt::CaseInsensitive) == 0 ){ return HSI_MINIMUM; }
    else if ( sMethod.compare("Weighted Mean", Qt::CaseInsensitive) == 0 ){ return HSI_WEIGHTED_MEAN; }

    SimulationError(SEVERITY_ERROR, "Could not determine Method for Raster combination in HSI Simulation");
    return -1;

}

void HSISimulation::Clean(){

}

// This function loads the HSC Inputs but it also creates copies of the
// Inputs for preparation down in PrepareInputs()
void HSISimulation::LoadInputs(){

    SimulationLog("Loading Inputs for HSI Simulation: " + GetName() , 2);

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
                m_NumRasters++;
            else if ( dynamic_cast <ProjectInputCSV *> ( pInput ))
                m_NumCSVs++;
            else if ( dynamic_cast <ProjectInputCSV *> ( pInput ))
                m_NumVectors++;

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
