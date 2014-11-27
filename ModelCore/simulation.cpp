#include <QString>

#include "simulation.h"
#include "hsi.h"
#include "project.h"

namespace HabitatModel{

Simulation::Simulation(QDomElement * elSimulation)
    : NamedObjectWithID(elSimulation, "Title", "SimulationID")
{

    //    Example Node:
    /*
    <Simulations>
      <SimulationID>3</SimulationID>
      <HSIID>3</HSIID>
      <CreatedBy>SB</CreatedBy>
      <CreatedOn>2014-07-10T00:00:00-07:00</CreatedOn>
      <VisitID>1</VisitID>
      <Discharge>0.5</Discharge>
      <IsQueuedToRun>false</IsQueuedToRun>
      <Title>Old2</Title>
      <AddIndividualOutput>false</AddIndividualOutput>
    </Simulations> */

    Project::GetOutputXML()->Log("Loading simulation: " + GetName() , 2);

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

void Simulation::RasterUnion(RasterManager::RasterMeta * pMeta){
    // First time round set the bounds to the first raster we give it.
    if (m_RasterTemplateMeta == NULL){
        m_RasterTemplateMeta = pMeta;
    }
    else {
        m_RasterTemplateMeta->Union(pMeta);
    }
}

void Simulation::Init(){
    m_RasterTemplateMeta = NULL;
    PrepareInputs();
}




}
