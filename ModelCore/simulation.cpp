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
    <SimulationID>2</SimulationID>
    <HSIID>3</HSIID>
    <CreatedBy>Matt</CreatedBy>
    <CreatedOn>2016-04-21T12:42:52.136752-07:00</CreatedOn>
    <IsQueuedToRun>true</IsQueuedToRun>
    <Title>test1</Title>
    <Folder>Simulations\test1</Folder>
    <AddIndividualOutput>false</AddIndividualOutput>
    <RunOn>1970-01-01T00:00:00-08:00</RunOn>
    <OutputRaster>Simulations\test1\Outputs\test1.tif</OutputRaster>
    <OutputCSV>Simulations\test1\Outputs\test1.csv</OutputCSV>
    <RasterTop>5007573</RasterTop>
    <RasterLeft>428470</RasterLeft>
    <RasterRows>2450</RasterRows>
    <RasterCols>2970</RasterCols>
    <RasterCellSize>0.1</RasterCellSize>
    <RasterSpatRef>TEMPORARY STRING FOR TESTING</RasterSpatRef>
    <RasterUnits>TEMPORARY STRING FOR TESTING</RasterUnits> */

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

    m_nHistogramBins = 20;

    QString OutputRasterPath = Project::SanitizePath(elSimulation->firstChildElement("OutputRaster").text());
    QString OutputCSVPath = Project::SanitizePath(elSimulation->firstChildElement("OutputCSV").text());

    m_bOutputRaster = "";
    m_bOutputCSV = "";

    if (OutputRasterPath.compare("",Qt::CaseInsensitive) != 0){
        m_bOutputRaster = Project::GetProjectRootPath()->filePath( OutputRasterPath );
        QString histogramBaseName = QFileInfo(m_bOutputRaster).baseName();
        QString histogramPath = QFileInfo(m_bOutputRaster).path();
        m_bOutputHistogram = Project::GetProjectRootPath()->filePath( QDir(histogramPath).filePath(histogramBaseName + "_histogram.csv") );
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

    // Simulation meta are ky/value pairs that get passed straight through the system
    LoadSimulationMeta();

    // Ratermeta is the the top/left/cellwidth/col/row data of the final raster
    InitRasterMeta(elSimulation);

    Init();

}
/*
<OutputRaster>Simulations\test1\Outputs\test1.tif</OutputRaster>
<OutputCSV>Simulations\test1\Outputs\test1.csv</OutputCSV>

<RasterRows>2450</RasterRows>
<RasterCols>2970</RasterCols>
<RasterSpatRef>TEMPORARY STRING FOR TESTING</RasterSpatRef>
<RasterUnits>TEMPORARY STRING FOR TESTING</RasterUnits> */
void Simulation::InitRasterMeta(QDomElement * elSimulation){

    // If there is no raster then just initialize a blank object and exit.
    if (!HasOutputRaster()){
        SimulationLog("No Raster Meta information needed. Skipping RasterMeta initiation.",2);
        m_RasterTemplateMeta = new RasterManager::RasterMeta();
        return;
    }

    double dTop = elSimulation->firstChildElement("RasterTop").text().toDouble();
    double dLeft = elSimulation->firstChildElement("RasterLeft").text().toDouble();
    double dCellWidth = elSimulation->firstChildElement("RasterCellSize").text().toDouble();
    double dCellHeight = -dCellWidth;
    int nRows = elSimulation->firstChildElement("RasterRows").text().toInt();
    int nCols = elSimulation->firstChildElement("RasterCols").text().toInt();

    QString qsProj = elSimulation->firstChildElement("RasterSpatRef").text();
    QString qsUnits = elSimulation->firstChildElement("RasterUnits").text();

    if (qsProj.compare("") == 0)
        SimulationLog("WARNING: Empty spatial ref detected for simulation",2);
    if (qsUnits.compare("") == 0)
        SimulationLog("WARNING: Empty units ref detected for simulation",2);

    const QByteArray qbXProjRef = qsProj.toLocal8Bit();
    const QByteArray qbXProjUnits = qsUnits.toLocal8Bit();
    const char * psProjection = qbXProjRef.data();
    const char * psUnits = qbXProjUnits.data();
    const char * psDriver = "GTiff";

    char * psWKT = NULL;
    psWKT = (char *) psProjection;
    char * psUnit = NULL;
    psUnit = (char *) psUnits;
    double fNoDataValue = (double) -std::numeric_limits<float>::max();

    OGRSpatialReference poSRS;
    poSRS.importFromWkt(&psWKT);
    poSRS.GetLinearUnits(&psUnit);
    GDALDataType nDType = GDT_Float32;
    m_RasterTemplateMeta = new RasterManager::RasterMeta(dTop, dLeft, nRows, nCols,
                                                         &dCellHeight, &dCellWidth,
                                                         &fNoDataValue, psDriver,
                                                         &nDType, psProjection, psUnit);
}

//<SimulationMeta>
//  <MetaID>1</MetaID>
//  <SimulationID>2</SimulationID>
//  <TheKey>sim1meta1</TheKey>
//  <TheValue>sim1metavalue1</TheValue>
//</SimulationMeta>

void Simulation::LoadSimulationMeta(){
    QDomNodeList elSimulationMetas =  Project::GetConfigDom()->elementsByTagName("SimulationMeta");
    for(int n= 0; n < elSimulationMetas.length(); n++){
        QDomElement elSimulationMeta = elSimulationMetas.at(n).toElement();
        int nSimulationID = elSimulationMeta.firstChildElement("SimulationID").text().toInt();
        if (nSimulationID > 0 && nSimulationID == GetID()){
            QString sKey = elSimulationMeta.firstChildElement("TheKey").text();
            QString sValue = elSimulationMeta.firstChildElement("TheValue").text();
            m_qhMetaValues.insert(sKey, sValue);
        }
    }
    // Nothing fancy here. Simulation meta passes straight through to the
    // log file
    m_XMLSimOutput->WriteSimulationMeta(this);
    Project::GetOutputXML()->WriteSimulationMeta(this);
}

Simulation::~Simulation() {
    delete m_RasterTemplateMeta;
    delete m_XMLSimOutput;
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

void Simulation::SimulationAddHistogram(RasterManager::HistogramsClass theHisto){
    m_XMLSimOutput->WriteHistogram(theHisto, this);
    Project::GetOutputXML()->WriteHistogram(theHisto, this);
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

}





}
