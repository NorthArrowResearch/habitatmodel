#include "fissimulation.h"

namespace HabitatModel{

FISSimulation::FISSimulation(QDomElement *elSimulation) : Simulation(elSimulation)
{
    QDomElement * elFIS = NULL;
    // Now Create our FIS object if there is one.
    int nSimulationFISID = elSimulation->firstChildElement("FISID").text().toInt();

    QDomNodeList elConfigFISs = Project::GetConfigDom()->elementsByTagName("FIS");

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
        Project::ProjectError(SEVERITY_ERROR, "Project is missing a <FIS>.");

    m_fis = new FIS(elFIS, &m_simulation_fis_inputs, GetRasterExtentMeta() );
    delete elFIS;

    // Make a local copy of each data source as a local simulation object,
    // ready for preparation.
    LoadInputs();

    // Now, if this thing is a raster we need to add it to the ExtentRectangle
    // For this simulation
    if (HasOutputRaster()){
        AddRastersToExtents();
    }

    QTime qtPrepTime;
    qtPrepTime.start();
    PrepareInputs();
    Project::GetOutputXML()->AddStatus(this->GetName(), STATUS_PREPARED, STATUSTYPE_SIMULATION, qtPrepTime.elapsed()/1000 );

}

FISSimulation::~FISSimulation()
{

}


void FISSimulation::AddRastersToExtents(){

    QHashIterator<int, SimulationFISInput *> i(m_simulation_fis_inputs);

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


void FISSimulation::LoadInputs()
{
    Project::GetOutputXML()->Log("Loading Inputs for FIS Simulation: " + GetName() , 2);

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
    Project::GetOutputXML()->Log("Starting FIS Simulation Run: " + GetName() , 0);


    // HERE'S WHERE FIS GETS RUN

//    if (m_dWeightedUse >= 0)
//        Project::GetOutputXML()->AddResult(this, "WeightedUsableArea",  QString::number(m_dWeightedUse) );


    Project::GetOutputXML()->AddStatus(this->GetName(), STATUS_COMPLETE, STATUSTYPE_SIMULATION , qtRunTime.elapsed()/1000);
    Project::GetOutputXML()->Log("FIS Simulation Complete", 1);

}


void FISSimulation::Clean()
{

}

void FISSimulation::PrepareInputs()
{
    Project::GetOutputXML()->Log("Preparing inputs for FIS Simulation: " + GetName() , 2);

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
