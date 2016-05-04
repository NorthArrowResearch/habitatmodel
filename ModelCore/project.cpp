#define MY_DLL_EXPORT

#include "project.h"
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QHash>
#include <QDir>
#include <QTime>

#include "habitat_exception.h"
#include "hsisimulation.h"
#include "fissimulation.h"
#include "hmvariable.h"
#include "hsccategorical.h"
#include "hscinflection.h"
#include "hmvariable.h"
#include "projectinputcsv.h"
#include "projectinputraster.h"
#include "projectinputvector.h"

namespace HabitatModel{

/* --------------------------------------------------------------- */
/* -- Static Variables need to be instantiated in CPP
      Check .h file for definition.                                */

QHash<int, HMVariable *> Project::m_hmvariable_store;
QHash<int, Unit *> Project::m_unit_store;
QHash<int, NamedObjectWithID *> Project::m_lookup_table;
QHash<int, ProjectInput *> Project::m_raw_project_inputs_store;
QHash<int, HSC *> Project::m_HSC_store;
QHash<int, Simulation *> Project::m_simulation_store;

QTime Project::m_totalTimer;
QTime Project::m_subprocessTimer;

XMLFile * Project::m_XMLInputDef;
XMLFile * Project::m_XMLInputConf;
XMLFile * Project::m_XMLOutput;

QDomDocument * Project::m_elConf;
QDomDocument * Project::m_elDef;

QDir * Project::m_ProjectRootDir;

/* --------------------------------------------------------------- */

Project::Project(const char * psProjectRoot,
                 const char * psXMLInputDef,
                 const char * psXMLInputConf,
                 const char * psXMLOutput)
{

    m_totalTimer.start();

    m_XMLInputDef = new XMLFile(psXMLInputDef, true);
    m_XMLInputConf = new XMLFile(psXMLInputConf, true);

    m_XMLOutput = new XMLFile(psXMLOutput, false);

    m_nSuccess = m_nFailures = m_nTotal = 0;

    Project::GetOutputXML()->Log("Opened Input Definitions XML File: " + QString(psXMLInputDef) );
    Project::GetOutputXML()->Log("Opened Input Configurations XML File: " + QString(psXMLInputConf) );
    Project::GetOutputXML()->Log("Opened Output XML File: " + QString(psXMLOutput) );

    // This is mostly a convenience: a pointer to the
    // Entire input Configuration DOM
    m_elConf = m_XMLInputConf->Document();
    m_elDef = m_XMLInputDef->Document();

    m_ProjectRootDir = new QDir(SanitizePath(psProjectRoot));
    if (!m_ProjectRootDir->exists())
        ProjectError( DIRECTORY_NOT_FOUND, m_ProjectRootDir->absolutePath() );


    m_XMLOutput->Log("Starting Simulation Run...");
    m_XMLOutput->AddStatus("Project", STATUS_INITIALIZED ,STATUSTYPE_PROJECT, m_totalTimer.elapsed()/1000);

    // Populate our lookup table hashes with values that every simulation
    // Will need access to.
    m_XMLOutput->Log("Loading Objects...", 1);
    LoadLookupTable();
    LoadHMVariables();
    LoadUnits();
    LoadHSCs();
    LoadProjectDataSources();

    m_XMLOutput->AddStatus("Project", STATUES_LOADED ,STATUSTYPE_PROJECT, 0);

    // Now load the simulations. This instantiates new Simulation objects
    // Based on the simulation type (HSI, FIS, etc) and each of those will
    // Load the models necessary to that particular simulation: HSI Curves etc.
    LoadSimulations();

    m_XMLOutput->AddRunMeta("LoadTime", QString::number(m_totalTimer.elapsed()/1000));
}


int Project::Run()
{
    m_XMLOutput->Log("Starting to run Simulations...");
    // Run the actual simulations. This is a polymorhic virtual function.
    QHashIterator<int, Simulation *> sim(m_simulation_store);
    while (sim.hasNext()) {
        sim.next();
        try{
            sim.value()->Run();
            m_nSuccess++;
        }
        catch(HabitatException e){
            sim.value()->SimulationError(e.GetErrorCode(), e.GetEvidence());
            m_nFailures++;
        }
    }
    m_XMLOutput->Log("All Simulations Completed.");
    m_XMLOutput->AddRunMeta("TotalTime", QString::number(m_totalTimer.elapsed()/1000));

    m_XMLOutput->AddRunMeta("Simulations", QString::number(m_nTotal));
    m_XMLOutput->AddRunMeta("Successes", QString::number(m_nSuccess));
    m_XMLOutput->AddRunMeta("Failures", QString::number(m_nFailures));

    m_XMLOutput->AddStatus("Project", STATUS_COMPLETE ,STATUSTYPE_PROJECT, m_totalTimer.elapsed()/1000);
    return PROCESS_OK;
}


void Project::LoadSimulations(){

    m_XMLOutput->Log("Loading Simulations...",1);
    QDomNodeList elSimulations =  m_elConf->elementsByTagName("Simulations");

    if (elSimulations.count() == 0)
        ProjectError(DOM_NODE_MISSING, "There are no <Simulations> in the configuration XML file.");

    // Loop over all Simulation elements in the XML file
    // ----------------------------------------------------------------------
    for(int n= 0; n < elSimulations.length(); n++){
        QDomElement elSimulation = elSimulations.at(n).toElement();
        QString sSimName = elSimulation.firstChildElement("Title").text();
        m_XMLOutput->AddStatus( sSimName, STATUS_INITIALIZED, STATUSTYPE_SIMULATION, 0 );
    }

    for(int n= 0; n < elSimulations.length(); n++){
        QDomElement elSimulation = elSimulations.at(n).toElement();

        // If it's not queued then we don't care about it.
        QString sQueued = elSimulation.firstChildElement("IsQueuedToRun").text();
        if (sQueued.compare("false", Qt::CaseInsensitive) != 0 && sQueued.compare("0", Qt::CaseInsensitive) != 0){
            int nSimulationID = elSimulation.firstChildElement("SimulationID").text().toInt();

            // Title gets processed formally later but this is helpful for any error messages
            QString s_SimulationTitle = elSimulation.firstChildElement("Title").text();
            if (s_SimulationTitle.trimmed().length() == 0)
                s_SimulationTitle = QString("Simulation with ID = %1").arg(nSimulationID);

            int nSimulationHSIID = elSimulation.firstChildElement("HSIID").text().toInt();
            // int nSimulationFISID = elSimulation.firstChildElement("FISID").text().toInt();

            bool bHSIID=false, bFISID=false;

            if (nSimulationHSIID > 0)
                bHSIID = true;
            else
                bFISID = true;

            Simulation * newSim;
            m_nTotal++;
            if (bHSIID){
                try{
                    newSim = new HSISimulation(&elSimulation);
                    m_simulation_store.insert(nSimulationID, newSim);
                }
                catch(HabitatException e){
                    GetOutputXML()->Log("Failed to initialize simulation: " + s_SimulationTitle, e.GetEvidence(), SEVERITY_ERROR, 1);
                    m_nFailures++;
                }
            }
            else if(bFISID){
                try{
                    newSim = new FISSimulation(&elSimulation);
                    m_simulation_store.insert(nSimulationID, newSim);
                }
                catch(HabitatException e){
                    GetOutputXML()->Log("Failed to initialize simulation: " + s_SimulationTitle, e.GetEvidence(), SEVERITY_ERROR, 1);
                    m_nFailures++;
                }
            }
            else{
                GetOutputXML()->Log("Missing HSI, FIS", "Simulation with ID '" + s_SimulationTitle + "' has no valid <HSI> or <FIS> nodes found in the config file. Skipping Simulation", SEVERITY_WARNING, 1);
            }
        }

    }

}

void Project::LoadProjectDataSources(){

    QDomNodeList elProjectInputs = m_elConf->elementsByTagName("ProjectDataSources");
    bool hasRasters = false;
    for(int n= 0; n < elProjectInputs.length(); n++){
        ProjectInput * p_projectinput;

        QDomElement elProjectInput = elProjectInputs.at(n).toElement();
        QString sInputFilepath = elProjectInput.firstChildElement("ProjectPath").text();

        int nProjectInputID = elProjectInput.firstChildElement("DataSourceID").text().toInt();
        int nproject_input_type = GetInputType(sInputFilepath);

        switch(nproject_input_type) {

        case PROJECT_INPUT_RASTER :
            p_projectinput = new ProjectInputRaster(&elProjectInput);
            m_raw_project_inputs_store.insert(nProjectInputID, p_projectinput);
            hasRasters = true;
            break;
        case PROJECT_INPUT_VECTOR :
            p_projectinput = new ProjectInputVector(&elProjectInput);
            m_raw_project_inputs_store.insert(nProjectInputID, p_projectinput);
            break;
        case PROJECT_INPUT_CSV :
            p_projectinput = new ProjectInputCSV(&elProjectInput);
            m_raw_project_inputs_store.insert(nProjectInputID, p_projectinput);
            break;
        case PROJECT_INPUT_UNDEFINED :
            ProjectError(FILE_NOT_FOUND, sInputFilepath);
            break;

        }
    }

}


HabitatModel::ProjectInputTypeCodes Project::GetInputType(QString sInputFilePath){

    if(sInputFilePath.endsWith(".tif") || sInputFilePath.endsWith(".tiff"))
        return PROJECT_INPUT_RASTER;

    else if(sInputFilePath.endsWith(".shp"))
        return PROJECT_INPUT_VECTOR;

    else if(sInputFilePath.endsWith(".csv"))
        return PROJECT_INPUT_CSV;

    else
        return PROJECT_INPUT_UNDEFINED;
}

void Project::LoadLookupTable(){

    m_XMLOutput->Log("Loading Lookup Table", 2);
    QDomNodeList elListItems = m_elDef->elementsByTagName("LookupListItems");

    //Loop through the LookipListItems nodes and load them into the hasj store
    for(int n= 0; n < elListItems.length(); n++){
        QDomNode elListItem = elListItems.at(n);
        int nListItemID = elListItem.firstChildElement("ItemID").text().toInt();
        QString sname = elListItem.firstChildElement("ItemName").text();
        const QByteArray QBsname = sname.toLocal8Bit();
        m_lookup_table.insert(nListItemID, new NamedObjectWithID(QBsname.data(), nListItemID));
    }

}

void Project::LoadUnits(){

    m_XMLOutput->Log("Loading Units", 2);
    QDomNodeList elUnits = m_elDef->elementsByTagName("Units");

    // Loop over all Unit elements in the XML file.
    for(int n= 0; n < elUnits.length(); n++){
        QDomElement elUnit = elUnits.at(n).toElement();
        int nListItemID = elUnit.firstChildElement("UnitID").text().toInt();
        m_unit_store.insert(nListItemID, new Unit(&elUnit));
    }
}

void Project::LoadHMVariables(){

    m_XMLOutput->Log("Loading HMVariables", 2);
    QDomNodeList elvars = m_elDef->elementsByTagName("Variables");

    // Loop over all HMVariable elements in the XML file
    for(int n= 0; n < elvars.length(); n++){
        QDomElement elvar = elvars.at(n).toElement();
        int nvarID = elvar.firstChildElement("VariableID").text().toInt();
        m_hmvariable_store.insert(nvarID, new HMVariable(&elvar));
    }
}

HSC * Project::LoadHSC(int nNewHSCID, int nType){
    // Create one if it doesn't exist.
    if ( !GetHSC(nNewHSCID) ){

        QDomNodeList elHSCs = m_elDef->elementsByTagName("HSC");

        for(int nc= 0; nc < elHSCs.length(); nc++){
            QDomElement elHSC = elHSCs.at(nc).toElement();
            int nHSCID = elHSC.firstChildElement("HSCID").text().toInt();
            if (nNewHSCID == nHSCID){
                HSC * p_newHSC;
                switch (nType) {
                case HSC_CATEGORICAL:
                    p_newHSC = new HSCCategorical(&elHSC);
                    m_HSC_store.insert(nHSCID, p_newHSC);
                    break;
                case HSC_INFLECTION:
                    p_newHSC = new HSCInflection(&elHSC);
                    m_HSC_store.insert(nHSCID, p_newHSC);
                    break;
                }
            }
        }
    }
    // Return the one we want.
    return m_HSC_store.value(nNewHSCID);
}

void Project::LoadHSCs(){

    m_XMLOutput->Log("Loading HSCs", 2);

    // Load first the coordinate pairs and then the HSC categories. If the parent
    // HSC doesn't exist it is created.
    QDomNodeList elHSCCoordPairs = m_elDef->elementsByTagName("HSCCoordinatePairs");

    for(int ncp= 0; ncp < elHSCCoordPairs.length(); ncp++){

        QDomElement elCoordinatePair = elHSCCoordPairs.at(ncp).toElement();
        int nCoordinatePairID = elCoordinatePair.firstChildElement("CoordinatePairID").text().toInt();
        int nHSCID = elCoordinatePair.firstChildElement("HSCID").text().toInt();

        HSCInflection * pHSCInflection = (HSCInflection*) LoadHSC(nHSCID, HSC_INFLECTION);
        HSCCoordinatePair coordpair(&elCoordinatePair);
        pHSCInflection->AddCoordinatePair(nCoordinatePairID, &coordpair);
    }


    // Now process the HSC Categories
    QDomNodeList elHSCCategories = m_elDef->elementsByTagName("HSCCategories");

    for(int ncat= 0; ncat < elHSCCategories.length(); ncat++){

        QDomElement elCategory = elHSCCategories.at(ncat).toElement();
        int nCatID = elCategory.firstChildElement("CategoryID").text().toInt();
        int nHSCID = elCategory.firstChildElement("HSCID").text().toInt();

        HSCCategorical * pHSCCategorical = (HSCCategorical*) LoadHSC(nHSCID, HSC_CATEGORICAL);
        HSCCategory CatVal(&elCategory);
        pHSCCategorical->AddCategory(nCatID, &CatVal);
    }

}


void Project::ProjectError(int nErrorCode){
    ProjectError(nErrorCode, "");
}

void Project::ProjectError(int nErrorCode, QString m_sEvidence){
    GetOutputXML()->Log(HabitatException::GetReturnCodeOnlyAsString(nErrorCode),  m_sEvidence, SEVERITY_ERROR, 1);
    throw HabitatException(nErrorCode, m_sEvidence);
}

QString Project::ReplaceSuffix(QString sPath, QString newSuffix){
    QFileInfo info(sPath);
    QString strNewName = info.path() + info.completeBaseName() + "." + newSuffix;
    return strNewName;
}

QString Project::SanitizePath(QString sPath){
    // It's possible this will need to be more complicated
    // later which is why we need a function for one command.
    sPath.replace("\\", "/");
    return sPath;
}


void Project::EnsureFile(QString sFilePath){

    QFileInfo sNewFileInfo(sFilePath);
    QDir sNewDir = QDir(sNewFileInfo.absolutePath());

    // Make a path if we don't have one already.
    if (!sNewDir.exists()){
        Project::GetOutputXML()->LogDebug("Dir Doesn't exist. Making : " + sNewDir.absolutePath() , 3);
        sNewDir.mkpath(".");
    }

    // Delete the file if it already exists.
    if (sNewFileInfo.exists()){
        Project::GetOutputXML()->LogDebug("File exists. Deleting : " + sNewFileInfo.fileName() , 3);
        QFile file(sFilePath);
        file.setPermissions(QFile::ReadOther | QFile::WriteOther);
        file.remove();
    }
}

Project::~Project(){

    // Empty the survey store
    QHashIterator<int, HMVariable *> i(m_hmvariable_store);
    while (i.hasNext()) {
        i.next();
        delete i.value();
    }
    m_hmvariable_store.clear();

    // Empty the unit store
    QHashIterator<int, Unit *> j(m_unit_store);
    while (j.hasNext()) {
        j.next();
        delete j.value();
    }
    m_unit_store.clear();

    // Empty the HSC store
    QHashIterator<int, HSC *> k(m_HSC_store);
    while (k.hasNext()) {
        k.next();
        delete k.value();
    }
    m_HSC_store.clear();

    // Empty the lookup table store
    QHashIterator<int, NamedObjectWithID *> l(m_lookup_table);
    while (l.hasNext()) {
        l.next();
        delete l.value();
    }
    m_lookup_table.clear();

    // Empty the project input store store
    QHashIterator<int, ProjectInput *> m(m_raw_project_inputs_store);
    while (m.hasNext()) {
        m.next();
        delete m.value();
    }
    m_raw_project_inputs_store.clear();

    // Empty the Simulation store
    QHashIterator<int, Simulation *> n(m_simulation_store);
    while (n.hasNext()) {
        n.next();
        delete n.value();
    }
    m_simulation_store.clear();

    delete m_ProjectRootDir;
    delete m_XMLInputDef;
    delete m_XMLInputConf;
    delete m_XMLOutput;
}



}
