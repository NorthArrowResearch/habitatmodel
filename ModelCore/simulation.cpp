#include <QString>

#include "simulation.h"
#include "hsi.h"
#include "project.h"
#include "habitat_exception.h"
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
    QString mSimRootFolder = Project::SanitizePath( (elSimulation->firstChildElement("Folder").text()) );

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

    m_NumRasters = 0;
    m_NumCSVs = 0;
    m_NumVectors = 0;

    QString OutputRasterPath = Project::SanitizePath(elSimulation->firstChildElement("OutputRaster").text());
    QString OutputCSVPath = Project::SanitizePath(elSimulation->firstChildElement("OutputCSV").text());

    m_bOutputRaster = "";
    m_bOutputCSV = "";

    if (OutputRasterPath.compare("",Qt::CaseInsensitive) != 0){
        m_bOutputRaster = Project::GetProjectRootPath()->filePath( OutputRasterPath );
    }

    if (OutputCSVPath.compare("",Qt::CaseInsensitive) != 0){
        m_bOutputCSV = Project::GetProjectRootPath()->filePath( OutputCSVPath );
    }

    if (!HasOutputRaster() && !HasOutputCSV()){
        throw HabitatException(XML_INPUT_ERROR, "OutputRaster and OutputCSV cannot both be empty");
    }
    if (HasOutputCSV() && m_NumCSVs > 1){
        throw HabitatException(XML_INPUT_ERROR, "CSV-only method does not support multiple CSVs at this time.");
    }

    m_dWeightedUse = -1;
    m_dNormWeightedUse = -1;
    m_dPercentUsage = -1;
    m_nOccupiedCells = -1;
    m_nTotalCells = -1;
    m_nCSVLines = -1;
    m_dCellArea = -1;

    // For CSV runs we need to know the cell size.
    double dCellSize = elSimulation->firstChildElement("CellSize").text().toDouble();
    if (dCellSize > 0)
        m_dCellSize = dCellSize;
    else
        m_dCellSize = -1;

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
