#define MY_DLL_EXPORT

#include "project.h"
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QHash>
#include <QDir>

#include "xmlfile.h"
#include "simulation.h"
#include "hsisimulation.h"
#include "hmvariable.h"
#include "unit.h"
#include "hsc.h"
#include "hsccategorical.h"
#include "hscinflection.h"
#include "hmvariable.h"
#include "projectinput.h"
#include "projectinputcsv.h"
#include "projectinputraster.h"
#include "projectinputvector.h"
#include "habitat_exception.h"

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

QDomElement Project::m_elConfig;
QDir * Project::m_ProjectRootDir;

/* --------------------------------------------------------------- */


Project::Project(const char * psProjectRoot,
                 const char * psXMLInput,
                 const char * psXMLOutput)
{

    XMLFile xmlInput(psXMLInput, true);
    XMLFile xmlOutput(psXMLOutput, false);

    // Load the entire input dom into a member variable.
    m_elConfig = xmlInput.Document()->documentElement();

    m_ProjectRootDir = new QDir(psProjectRoot);
    if (!m_ProjectRootDir->exists())
        throw HabitatException(DIRECTORY_NOT_FOUND, m_ProjectRootDir->absolutePath());

//    QDir sXMLConfigDir = QFileInfo(sXMLConfig).absoluteDir();
//    m_ConfigPath = &sXMLConfigDir;
//    m_ProjectDir = &sXMLConfigDir;

    // TODO:: Set up folders:
    // 1. Read the output log file from the simulation node and

    // Make a temporary folder for this simulation
//    if (m_ConfigPath->mkdir("tmp")){
//        QDir tmpPath = QDir(m_ConfigPath->absolutePath() + QDir::separator() + "tmp");
//        m_TmpPath = new QDir(tmpPath);
//    }
//    else {
//        throw std::runtime_error(std::string("Could not create \"tmp\" temporary at: \"" + m_ConfigPath->absolutePath().toStdString() + "\". Does it already exist?"));
//    }


    // Populate our lookup table hashes with values that every simulation
    // Will need access to.
//    LoadLookupTable();
//    LoadUnits();
//    LoadHSCs();
//    LoadHMVariables();
//    LoadProjectInputs();

    // Now load the simulations. This instantiates new Simulation objects
    // Based on the simulation type (HSI, FIS, etc) and each of those will
    // Load the models necessary to that particular simulation: HSI Curves etc.
//    LoadSimulations();

}


int Project::Run()
    {

    // Run the actual simulations. This is a polymorhic virtual function.
    QHashIterator<int, Simulation *> sim(m_simulation_store);
    while (sim.hasNext()) {
        sim.next();
        sim.value()->Run();
    }

    return PROCESS_OK;
}


void Project::LoadSimulations(){

     QDomNodeList elSimulations =  m_elConfig.elementsByTagName("Simulations");

    if (elSimulations.count() == 0)
        throw std::runtime_error("There are no <Simulations> in the configuration XML file.");

    // Loop over all Simulation elements in the XML file
    // ----------------------------------------------------------------------
    for(int n= 0; n < elSimulations.length(); n++){
        QDomElement elSimulation = elSimulations.at(n).toElement();

        int nSimulationID = elSimulation.firstChildElement("SimulationID").text().toInt();

        int nSimulationHSIID = elSimulation.firstChildElement("HSIID").text().toInt();
        int nSimulationFISID = elSimulation.firstChildElement("FISID").text().toInt();

        bool bHSIID=false, bFISID=false;

        QDomNodeList elConfigHSIs = m_elConfig.elementsByTagName("HSI");

        for(int n= 0; n < elConfigHSIs.length(); n++){
            QDomElement elHSI = elConfigHSIs.at(n).toElement();
            int nTestHSIID = elHSI.firstChildElement("HSIID").text().toInt();
            if (nSimulationHSIID == nTestHSIID)
                bHSIID = true;
        }

        Simulation * newSim;

        if (bHSIID){
            newSim = new HSISimulation(&elSimulation);
            m_simulation_store.insert(nSimulationID, newSim);
        }
        else if(bFISID){
            // m_simulation_store.insert(nSimulationID, new HSISimulation(&elSimulation));
        }
        else{
            throw std::runtime_error("No valid <HSI> or <FIS> nodes found in the config file.");
        }

    }

}

void Project::LoadProjectInputs(){

    QDomNodeList elProjectInputs = m_elConfig.elementsByTagName("ProjectInputs");

    for(int n= 0; n < elProjectInputs.length(); n++){
        ProjectInput * p_projectinput;

        QDomElement elProjectInput = elProjectInputs.at(n).toElement();
        QString sInputFilepath = elProjectInput.firstChildElement("SourcePath").text();

//      int nProjectInputID = elProjectInput.firstChildElement("InputID").text().toInt();
        int nproject_type = GetInputType(sInputFilepath);

        switch(nproject_type) {

        case PROJECT_INPUT_RASTER :
            p_projectinput = new ProjectInputRaster(&elProjectInput);
            m_raw_project_inputs_store.insert(n, p_projectinput);
            break;
        case PROJECT_INPUT_VECTOR :
            p_projectinput = new ProjectInputVector(&elProjectInput);
            m_raw_project_inputs_store.insert(n, p_projectinput);
            break;
        case PROJECT_INPUT_CSV :
            p_projectinput = new ProjectInputCSV(&elProjectInput);
            m_raw_project_inputs_store.insert(n, p_projectinput);
            break;
        case PROJECT_INPUT_UNDEFINED :
            throw std::runtime_error(std::string("ERROR: No valid input file detected ") + sInputFilepath.toStdString());
            break;

        }
    }
}

HabitatModel::ProjectInputTypeCodes Project::GetInputType(QString sInputFilePath){

    if(sInputFilePath.endsWith(".tif") || sInputFilePath.endsWith(".tiff")) {
        return PROJECT_INPUT_RASTER;
    }
    else if(sInputFilePath.endsWith(".shp")){
        return PROJECT_INPUT_VECTOR;
    }
    else if(sInputFilePath.endsWith(".csv")){
        return PROJECT_INPUT_CSV;
    }
    else {
        return PROJECT_INPUT_UNDEFINED;
    }
}

void Project::LoadLookupTable(){

    QDomNodeList elListItems = m_elConfig.elementsByTagName("LookupListItems");

    //Loop through the LookipListItems nodes and load them into the hasj store
    for(int n= 0; n < elListItems.length(); n++){
        QDomNode elListItem = elListItems.at(n);
        int nListItemID = elListItem.firstChildElement("ItemID").text().toInt();
        QString sname = elListItem.firstChildElement("ItemName").text();
        m_lookup_table.insert(nListItemID, new NamedObjectWithID(sname.toStdString().c_str(), nListItemID));
    }

}

void Project::LoadUnits(){
    QDomNodeList elUnits = m_elConfig.elementsByTagName("Units");

    // Loop over all Unit elements in the XML file.
    for(int n= 0; n < elUnits.length(); n++){
        QDomElement elUnit = elUnits.at(n).toElement();
        int nListItemID = elUnit.firstChildElement("UnitID").text().toInt();
        m_unit_store.insert(nListItemID, new Unit(&elUnit));
    }
}

void Project::LoadHMVariables(){
    QDomNodeList elvars = m_elConfig.elementsByTagName("Variables");

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

        QDomNodeList elHSCs = m_elConfig.elementsByTagName("HSC");

        for(int nc= 0; nc < elHSCs.length(); nc++){
            QDomElement elHSC = elHSCs.at(nc).toElement();
            int nHSCID = elHSC.firstChildElement("HSCID").text().toInt();
            if (nNewHSCID == nHSCID){
                HSC * p_newHSC;
                switch (nType) {
                case HSC_CATEGORICAL:
                    p_newHSC = new HSCInflection(&elHSC);
                    m_HSC_store.insert(nHSCID, p_newHSC);
                    break;
                case HSC_INFLECTION:
                    p_newHSC = new HSCCategorical(&elHSC);
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

    // Load first the coordinate pairs and then the HSC categories. If the parent
    // HSC doesn't exist it is created.
    QDomNodeList elHSCCoordPairs = m_elConfig.elementsByTagName("HSCCoordinatePairs");

    for(int ncp= 0; ncp < elHSCCoordPairs.length(); ncp++){
        QDomElement elCoordinatePair = elHSCCoordPairs.at(ncp).toElement();
        int nCoordinatePairID = elCoordinatePair.firstChildElement("CoordinatePairID").text().toInt();
        int nHSCID = elCoordinatePair.firstChildElement("HSCID").text().toInt();

        HSCInflection * pHSCInflection = (HSCInflection*) LoadHSC(nHSCID, HSC_INFLECTION);
        pHSCInflection->AddCoordinatePair(nCoordinatePairID, new HSCCoordinatePair(&elCoordinatePair));
    }

    QDomNodeList elHSCCategories = m_elConfig.elementsByTagName("HSCCategories");

    for(int ncat= 0; ncat < elHSCCategories.length(); ncat++){
        QDomElement elCategory = elHSCCategories.at(ncat).toElement();
        int nCatID = elCategory.firstChildElement("CategoryID").text().toInt();
        int nHSCID = elCategory.firstChildElement("HSCID").text().toInt();

        HSCCategorical * pHSCCategorical = (HSCCategorical*) LoadHSC(nHSCID, HSC_CATEGORICAL);
        pHSCCategorical->AddCategory(nCatID, new HSCCategory(&elCategory));
    }

}


Project::~Project(){



    // Empty the survey store
    QHashIterator<int, HMVariable *> i(m_hmvariable_store);
    while (i.hasNext()) {
        i.next();
        delete i.value();
    }

    // Empty the unit store
    QHashIterator<int, Unit *> j(m_unit_store);
    while (j.hasNext()) {
        j.next();
        delete j.value();
    }

    // Empty the HSC store
    QHashIterator<int, HSC *> k(m_HSC_store);
    while (k.hasNext()) {
        k.next();
        delete k.value();
    }

    // Empty the lookup table store
    QHashIterator<int, NamedObjectWithID *> l(m_lookup_table);
    while (l.hasNext()) {
        l.next();
        delete l.value();
    }

    // Empty the project input store store
    QHashIterator<int, ProjectInput *> m(m_raw_project_inputs_store);
    while (m.hasNext()) {
        m.next();
        delete m.value();
    }

    // Empty the Simulation store
    QHashIterator<int, Simulation *> n(m_simulation_store);
    while (n.hasNext()) {
        n.next();
        delete n.value();
    }

    delete m_ProjectRootDir;
}



}
