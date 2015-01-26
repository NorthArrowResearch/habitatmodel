#include <QString>

#include "simulation.h"
#include "hsi.h"
#include "project.h"
#include "rastermanager_interface.h"

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
    QString mSimRootFolder = Project::SanitizePath((elSimulation->firstChildElement("Folder").text()));

    m_sfolder =  Project::GetProjectRootPath()->filePath(mSimRootFolder);

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

    QString OutputRaster = elSimulation->firstChildElement("OutputRaster").text();
    QString OutputCSV = elSimulation->firstChildElement("OutputCSV").text();

    m_bOutputRaster = false;
    m_bOutputCSV = false;

    if (OutputRaster == "true" || OutputRaster == "1")
        m_bOutputRaster = true;

    if (OutputCSV == "true" || OutputCSV == "1")
        m_bOutputCSV = true;


    if (!m_bOutputCSV && !m_bOutputCSV){
        throw HabitatException(XML_INPUT_ERROR, "OutputRaster and OutputCSV cannot both be false");
    }

    Init();

}

Simulation::~Simulation() {
    delete m_RasterTemplateMeta;
}

void Simulation::RasterUnion(RasterManager::RasterMeta * pMeta){

    // First time round set the bounds to the first raster we give it.
    if (m_RasterTemplateMeta == NULL){
        m_RasterTemplateMeta = new RasterManager::RasterMeta(*pMeta);
    }
    else {
        m_RasterTemplateMeta->Union(pMeta);
    }
}

void Simulation::Init(){
    m_RasterTemplateMeta = NULL;
}





}
