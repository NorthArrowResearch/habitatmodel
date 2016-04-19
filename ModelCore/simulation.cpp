#include <QString>
#include "gdal_priv.h"
#include "simulation.h"
#include "hsi.h"
#include "project.h"
#include "habitat_exception.h"
#include "rastermanager_interface.h"
#include "ogrsf_frmts.h"

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

    // Simulations get their own special XML file.
    QString sHSResultsLog = QDir(m_sfolder).filePath("HSResults.xml");
    Project::EnsureFile(sHSResultsLog);
    m_XMLSimOutput = new XMLFile(sHSResultsLog, false);

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

    if (this->HasOutputRaster()){
        InitRasterMeta(elSimulation);
    }

    Init();

}

void Simulation::InitRasterMeta(QDomElement * elSimulation){

    double dTop = elSimulation->firstChildElement("top").text().toDouble();
    double dLeft = elSimulation->firstChildElement("left").text().toDouble();
    double dCellSize = elSimulation->firstChildElement("cellsize").text().toDouble();
    int nRows = elSimulation->firstChildElement("rows").text().toInt();
    int nCols = elSimulation->firstChildElement("cols").text().toInt();

    QString qsProj = elSimulation->firstChildElement("projection").text();
    const QByteArray qbXProj = qsProj.toLocal8Bit();
    const char * psProjection = qbXProj.data();
    const char * psDriver = "GTiff";

    char * psUnit = NULL;
    char * psWKT = NULL;
    psWKT = (char *) psProjection;
    double fNoDataValue = (double) -std::numeric_limits<float>::max();

    OGRSpatialReference poSRS;
    poSRS.importFromWkt(&psWKT);
    poSRS.GetLinearUnits(&psUnit);
    GDALDataType nDType = GDT_Float32;
    m_RasterTemplateMeta = new RasterManager::RasterMeta(dTop, dLeft, nRows, nCols,
                                                                            &dCellSize, &dCellSize,
                                                                            &fNoDataValue, psDriver,
                                                                            &nDType, psProjection, psUnit);
}

Simulation::~Simulation() {
    delete m_RasterTemplateMeta;
}

void Simulation::SimulationError(int nErrorCode){
    SimulationError(nErrorCode, "");
    Project::ProjectError(nErrorCode, "");
}

void Simulation::SimulationError(int nErrorCode, QString m_sEvidence){
    m_XMLSimOutput->Log(HabitatException::GetReturnCodeOnlyAsString(nErrorCode),  m_sEvidence, SEVERITY_ERROR, 1);
    Project::ProjectError(nErrorCode, m_sEvidence);
}

void Simulation::SimulationLog(QString sMsg, int nIndent){
    m_XMLSimOutput->Log(sMsg, nIndent);
    Project::GetOutputXML()->Log(sMsg, nIndent);
}

void Simulation::SimulationAddResult(QString sTagName, QString sTagValue){
    m_XMLSimOutput->AddResult(this, sTagName,  sTagValue);
    Project::GetOutputXML()->AddResult(this, sTagName,  sTagValue);
}

/**
 * @brief Simulation::RasterUnion DEPPRECATED FOR NOW since we're doing this work in the
 * @param pMeta
 */
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
