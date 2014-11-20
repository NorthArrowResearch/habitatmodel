#include <QString>

#include "simulation.h"
#include "hsi.h"
#include "project.h"

namespace HabitatModel{

Simulation::Simulation(QDomElement * elSimulation)
    : NamedObjectWithID(elSimulation, "Title", "SimulationID")
{

    //    Example Node:
    //    <Simulations>
    //    <SimulationID>4</SimulationID>
    //    <HSIID>3</HSIID>
    //    <CreatedBy>James</CreatedBy>
    //    <CreatedOn>2014-10-17T14:03:00.676-06:00</CreatedOn>
    //    <IsQueuedToRun>true</IsQueuedToRun>
    //    <HSISourcePath>HSI Source Path</HSISourcePath>
    //    <Title>ChinookSpawner</Title>
    //    <Folder>C:\Users\A01674762\Desktop\ScratchWorkspace\HabitatSuitabilityModeling\JohnStockton\Simulations\ChinookSpawner</Folder>
    //    <AddIndividualOutput>true</AddIndividualOutput>
    //    </Simulations>

    // First set all the member variables according to what's in the XML
    m_sfolder = elSimulation->firstChildElement("Folder").text();
    m_shsi_source_path = elSimulation->firstChildElement("HSISourcePath").text();

    QString sQueued = elSimulation->firstChildElement("IsQueuedToRun").text();
    if (sQueued == "true" || sQueued == "1")
        m_bqueued = true;
    else
        m_bqueued = false;

    QString sAddIndOut = elSimulation->firstChildElement("AddIndividualOutput").text();
    if (sAddIndOut == "true" || sAddIndOut == "1")
        m_badd_individual_output = true;
    else
        m_badd_individual_output = false;

    m_screated_on = elSimulation->firstChildElement("CreatedOn").text();
    m_screated_by = elSimulation->firstChildElement("CreatedBy").text();

    Init();

}

void Simulation::Init(){

    PrepareProjectInputs();

}

bool Simulation::InputBelongs(ProjectInput){



}

void Simulation::PrepareProjectInputs(){
    // This is a 3 step process:
    // 1). Go and get the appropriate project inputs
    // 2). Figure out the raster extents of these inputs
    // 3). Prepare them based on this extent.
    QHash<int, ProjectInput *> pRawInputStore = Project::GetRawProjectInputsStore();
    // TODO: need to figure out how to have prepared inputs for each simulation
    // Calculate the raster union and make rasters from CSV

    // First do the Rasters to find the union intersection
    // RasterMeta
    QHashIterator<int, ProjectInput *> rInputs(pRawInputStore);
    bool bFirst = true;
    while (rInputs.hasNext()) {
        rInputs.next();

        if (InputBelongs(rInputs.value())){
            // This input is part of our simulation. add it to the store.
            m_processed_inputs_store.insert(rInputs.value()->GetID(), rInputs.value());

            if (dynamic_cast<ProjectInputRaster*>(rInputs.value())){
                // Load the raster.
                RasterManager::RasterMeta erRasterInput (rInputs.value()->getSourceFilePath().toStdString().c_str());
                // First time round set the bounds to the first raster we give it.
                if (bFirst){
                    RasterManager::RasterMeta startingRect (erRasterInput);
                    m_RasterTemplateMeta = &startingRect;
                    bFirst = false;
                }
                else {
                    m_RasterTemplateMeta->Union(&erRasterInput);
                }
            }
        }
    }
    rInputs.toFront();

    // Next Call Prepare on Each Raster
    while (rInputs.hasNext()) {
        rInputs.next();
        rInputs.value()->Prepare(m_RasterTemplateMeta, );
    }

}


}
