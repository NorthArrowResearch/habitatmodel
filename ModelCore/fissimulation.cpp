#include "fissimulation.h"
#include "projectinputraster.h"
#include "habitat_exception.h"
#include "rastermanager_interface.h"
#include "QTextStream"

namespace HabitatModel{

FISSimulation::FISSimulation(QDomElement *elSimulation) : Simulation(elSimulation)
{

    QDomElement * elFIS = NULL;
    // Now Create our FIS object if there is one.
    int nSimulationFISID = elSimulation->firstChildElement("FISID").text().toInt();

    QDomNodeList elConfigFISs = Project::GetDefDom()->elementsByTagName("FIS");

    bool bFISFound = false;
    for(int n= 0; n < elConfigFISs.length(); n++){
        if (!bFISFound){
            elFIS = new QDomElement(elConfigFISs.at(n).toElement());
            int nTestFISID = elFIS->firstChildElement("FISID").text().toInt();
            if (nSimulationFISID == nTestFISID)
                bFISFound = true;
        }
    }

    if (elFIS == NULL)
        SimulationError(SEVERITY_ERROR, "Project is missing a <FIS> tag.");

    // Make a local copy of each data source as a local simulation object,
    // ready for preparation.
    LoadInputs();

    // Now, if this thing is a raster we need to add it to the ExtentRectangle
    // For this simulation
    if (HasOutputRaster()){
        AddRastersToExtents();
    }


    QString sFISRulePath = Project::SanitizePath(elFIS->firstChildElement("FISRuleFile").text());
    m_sFISRuleFile = Project::GetProjectRootPath()->filePath(sFISRulePath);

    if (!QFile(m_sFISRuleFile).exists())
        SimulationError(FILE_NOT_FOUND, QString("Could not find the FIS Rule File: %1").arg(m_sFISRuleFile));

    delete elFIS;

    QTime qtPrepTime;
    qtPrepTime.start();
    PrepareInputs();
    Project::GetOutputXML()->AddStatus(this->GetName(), STATUS_PREPARED, STATUSTYPE_SIMULATION, qtPrepTime.elapsed()/1000 );


    rules = new FISRuleSet();
    // Load the FIS rule file
    rules->parseFile(m_sFISRuleFile);

    // Confirm that the number of inputs specified matches the number in the rule file
    if (rules->numInputs() != m_simulation_fis_inputs.count())
        throw HabitatException(FIS_ERROR, "Number of FIS inputs must match the number in the rule file.");


}

FISSimulation::~FISSimulation()
{

}


/*
 * Opens datasets for reading and writing, allocates memory, and figures out if we need to check
 * for NODATA values in the input datasets. Most of the variables used in this method are class
 * members. The Datasources are set in the constructor. The Datasets opened here must be closed
 * in the finalizeProcess() method, especially the ones opened for writing (or data may not get
 * written properly). The arrays allocated here MUST be freed in the finalizeProcess() method
 * unless we want to introduce a memory leak.
 */
void FISSimulation::RunRasterFis(QString sOutputFile)
{
    Project::GetOutputXML()->Log("Beginning Raster FIS Simulation run: " + sOutputFile , 2);

    QHashIterator<int, SimulationFISInput *> dSimFISInputs(m_simulation_fis_inputs);
    QHash<int, GDALRasterBand *> dDatasets;
    QHash<int, double *> dInBuffers;
    std::vector<double> inputNoDataValues = std::vector<double>(m_simulation_fis_inputs.count());
    std::vector<double> inputData = std::vector<double>(m_simulation_fis_inputs.count());

    bool checkNoData = GetRasterExtentMeta()->HasNoDataValue();
    double dNodataVal = GetRasterExtentMeta()->GetNoDataValue();

    int sRasterCols = GetRasterExtentMeta()->GetCols();

    // Open all the inputs into a hash of datasets. We must remember to clean this up later

    if (rules->numInputs() != m_simulation_fis_inputs.count())
        throw new HabitatException(FIS_ERROR, "Number of inputs does not match number of FIS inputs.");

    Project::GetOutputXML()->Log(QString("Preparing %1 inputs for FIS.").arg(m_simulation_fis_inputs.count()), 3 );
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
    GDALDataset * pOutputDS = RasterManager::CreateOutputDS( sFISOutputQB.data(), GetRasterExtentMeta());
    GDALRasterBand * pOutputRB = pOutputDS->GetRasterBand(1);
    double * pReadBuffer = (double*) CPLMalloc(sizeof(double) * sRasterCols);

    // See if we need to worry about NODATA in the inputs.
    double * pOutputBuffer = (double*) CPLMalloc(sizeof(double) * sRasterCols);

    rules->initFuzzy();

    // Loop over the rows and columns. DO FIS!!
    double cellSum = 0;
    double usedCellCounter = 0;
    double cellArea = fabs( GetRasterExtentMeta()->GetCellHeight() * GetRasterExtentMeta()->GetCellWidth() );

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

            QHashIterator<int, GDALRasterBand *> dDatasetIterator(dDatasets);
            while (dDatasetIterator.hasNext()) {
                dDatasetIterator.next();
                int nDataKey = dDatasetIterator.key();
                inputData[nDataKey] = dInBuffers.value(nDataKey)[j];
            }           

            pOutputBuffer[j] = rules->calculate(inputData, checkNoData,
                                                inputNoDataValues,
                                                dNodataVal);

            if (pOutputBuffer[j] != dNodataVal){
                cellSum += pOutputBuffer[j];
                usedCellCounter++;
            }

        }

        pOutputRB->RasterIO(GF_Write, 0, i,
                            sRasterCols, 1,
                            pOutputBuffer,
                            sRasterCols, 1,
                            GDT_Float64,
                            0, 0 );

    }

    // Now write some results
    m_dCellArea = cellArea;
    m_dWeightedUse = cellSum * cellArea;
    m_dNormWeightedUse = m_dWeightedUse / (usedCellCounter * cellArea);
    m_dPercentUsage = 100 * usedCellCounter / ( GetRasterExtentMeta()->GetRows() * GetRasterExtentMeta()->GetCols() );
    m_nOccupiedCells = usedCellCounter;

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

void FISSimulation::RunCSVFis(QString sOutputFile)
{
    Project::GetOutputXML()->Log("Beginning CSV input FIS Run: " + sOutputFile , 2);

    QString sXField, sYField, sInputCSVFile;
    QHashIterator<int, SimulationFISInput *> dSimFISInputs( m_simulation_fis_inputs);

    // The first in tis the rule file index. The second int is the csv column index
    QHash<int, int> qhFISInputOrder;
    double dNoDataVal = std::numeric_limits<double>::min();
    std::vector<double> inputNoDataValues = std::vector<double>(m_simulation_fis_inputs.count());
    std::fill (inputNoDataValues.begin(),inputNoDataValues.end(),dNoDataVal);
    std::vector<double> inputData = std::vector<double>(m_simulation_fis_inputs.count());

    Project::GetOutputXML()->Log(QString("Preparing %1 inputs for FIS.").arg(m_simulation_fis_inputs.count()), 3 );

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

    // Create and open a new CSV file for writing
    // --------------------------------------------
    Project::EnsureFile(sOutputFile);
    QFile outputCSVFile(sOutputFile);

    if (!outputCSVFile.open(QFile::WriteOnly|QFile::Truncate)){
        throw HabitatException(FILE_WRITE_ERROR, "Could not open file CSV output file for writing: " + sOutputFile  );
    }
    QTextStream qtsOutputStream(&outputCSVFile);



    rules->initFuzzy();

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
                        QString sFISInputName = dSimFISInputs.value()->GetFISInputName();
                        ProjectInputCSV * InputCSV = dynamic_cast <ProjectInputCSV *> ( pInput );

                        if ( InputCSV && sCSVCol.compare(InputCSV->GetValueFieldName(), Qt::CaseInsensitive) == 0 ){

                            // add this input to our list
                            slCSVInputs.append(sCSVCol);
                            for (int i=0; i < rules->numInputs(); i++){
                                QString sRulesInputname = QString::fromUtf8( rules->getInputName(i) );
                                if (sRulesInputname.compare(sFISInputName, Qt::CaseInsensitive) == 0)
                                    qhFISInputOrder.insert(nColNumber, i);
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
                Project::ProjectError(CSV_INPUT_ERROR, "Could not find X-col " + sXField + " in CSV file.");

            if (ycol >= 0){
                QString yVal = slCSVcells.at(ycol);
                ProjectInputCSV::CSVCellClean( yVal );
                slCSVInputs.append(yVal);
            }
            else
                Project::ProjectError(CSV_INPUT_ERROR, "Could not find Y-col " + sYField + " in CSV file.");

            // Now loop through all cols and add the ones we need.
            foreach (QString sCSVCol, slCSVcells){

                if (qhFISInputOrder.find(nColNumber).key() == nColNumber){
                    int nColRule = qhFISInputOrder.find(nColNumber).value();
                    // Now add the Data input value
                    ProjectInputCSV::CSVCellClean( sCSVCol );
                    slCSVInputs.append(sCSVCol);

                    if (double dCSVItem = sCSVCol.toDouble() ){
                        if (dCSVItem != dNoDataVal && sCSVCol.length() > 0){
                            inputData[nColRule] =  dCSVItem;
                        }
                        else{
                            inputData[nColRule] = dNoDataVal;
                            slCSVOutputs.append(" ");
                        }
                    }
                    // Could not make a double. Just insert a blank space
                    else {
                        inputData[nColRule] = dNoDataVal;
                        slCSVOutputs.append(" ");
                    }

                }

                nColNumber++;
            }

            double dCombinedVal = rules->calculate(inputData, true,
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
    m_dCellArea = m_dCellSize * m_dCellSize;
    m_dWeightedUse = cellSum * m_dCellArea;
    m_dNormWeightedUse = m_dWeightedUse / (usedCellCounter * m_dCellArea);
    m_nOccupiedCells = usedCellCounter;
    m_nCSVLines = nlinenumber;


    // Note: Percent usage is not a useful stat here so it is not written.
    InputCSVFile.close();
    outputCSVFile.close();
}




void FISSimulation::AddRastersToExtents(){

    QHashIterator<int, SimulationFISInput *> i(m_simulation_fis_inputs);

    while (i.hasNext()) {
        i.next();
        // Here is the curve we want
        ProjectInput * pInput = i.value()->GetProjectInput();

        if ( dynamic_cast <ProjectInputRaster *> ( pInput )){
            try {
                SimulationLog("Adding Raster to extent: " + i.value()->GetProjectInput()->GetName() , 2);
                QString sRasterPath = pInput->GetSourceFilePath();
                const QByteArray QBRasterPath = sRasterPath.toLocal8Bit();

                RasterManager::RasterMeta * pRasterMeta = new RasterManager::RasterMeta(QBRasterPath.data());
                RasterUnion(pRasterMeta);
                delete pRasterMeta;
            }
            catch (RasterManager::RasterManagerException e){
                SimulationLog("ERROR:" + e.GetReturnMsgAsString() , 0);
            }

        }
    }
}


void FISSimulation::LoadInputs()
{
    SimulationLog("Loading Inputs for FIS Simulation: " + GetName() , 2);

    QDomNodeList elFISInputs = Project::GetConfigDom()->elementsByTagName("SimulationFISInputs");
    for(int n= 0; n < elFISInputs.length(); n++){
        QDomElement elFISInput = elFISInputs.at(n).toElement();

        // Test to see that our FISInputs belong to this simulation.
        int nFISSimulationID = elFISInput.firstChildElement("SimulationID").text().toInt();
        if (GetID() == nFISSimulationID){
            SimulationFISInput * newFISInput = new SimulationFISInput(elFISInput);

            ProjectInput * pInput = newFISInput->GetProjectInput();
            if ( dynamic_cast <ProjectInputRaster *> ( pInput ))
                m_NumRasters++;
//            else if ( dynamic_cast <ProjectInputCSV *> ( pInput ))
//                m_NumCSVs++;
//            else if ( dynamic_cast <ProjectInputCSV *> ( pInput ))
//                m_NumVectors++;

            m_simulation_fis_inputs.insert(n, newFISInput);
        }

    }

}

void FISSimulation::Run()
{
    /**
     *
     *  Get and loop over all the simulationHSCInputs, comparing them to
     *  their curves along the way.
     *
     **/
    QTime qtRunTime;
    qtRunTime.start();
    SimulationLog("Starting FIS Simulation Run: " + GetName() , 0);

    if (HasOutputRaster()){
        Project::EnsureFile(m_bOutputRaster);
        RunRasterFis(m_bOutputRaster);
    }

    if (HasOutputCSV() && !HasOutputRaster()){
        Project::EnsureFile(m_bOutputCSV);
        RunCSVFis(m_bOutputCSV);
    }
    if (m_dWeightedUse >= 0)
        Project::GetOutputXML()->AddResult(this, "WeightedUsableArea",  QString::number(m_dWeightedUse) );
    if (m_dNormWeightedUse >= 0)
        Project::GetOutputXML()->AddResult(this, "NormalizedWeightedUsableArea",  QString::number(m_dNormWeightedUse) );
    if (m_dPercentUsage >= 0)
        Project::GetOutputXML()->AddResult(this, "PercentOccupied",  QString::number(m_dPercentUsage) );
    if (m_nOccupiedCells >= 0)
        Project::GetOutputXML()->AddResult(this, "OccupiedCells",  QString::number(m_nOccupiedCells) );
    if (m_nTotalCells >= 0)
        Project::GetOutputXML()->AddResult(this, "TotalCells",  QString::number(m_nTotalCells) );
    if (m_nCSVLines >= 0)
        Project::GetOutputXML()->AddResult(this, "CSVLines",  QString::number(m_nCSVLines) );
    if (m_dCellArea >= 0)
        Project::GetOutputXML()->AddResult(this, "CellArea",  QString::number(m_dCellArea) );


    Project::GetOutputXML()->AddStatus(this->GetName(), STATUS_COMPLETE, STATUSTYPE_SIMULATION , qtRunTime.elapsed()/1000);
    SimulationLog("FIS Simulation Complete", 1);

}


void FISSimulation::Clean()
{

}

void FISSimulation::PrepareInputs()
{
    SimulationLog("Preparing inputs for FIS Simulation: " + GetName() , 2);

    QHashIterator<int, SimulationFISInput *> i(m_simulation_fis_inputs);
    while (i.hasNext()) {
        i.next();
        Simulation * pThisSim = this;
        // A purely CSV output request needs no preparing of outputs
        if (HasOutputRaster())
            i.value()->GetProjectInput()->Prepare(pThisSim);
    }
}

}
