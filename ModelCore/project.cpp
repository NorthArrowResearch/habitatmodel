#include "project.h"
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QHash>
#include <QDir>

#include "xmlfilehander.h"
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

namespace HabitatModel{

/* --------------------------------------------------------------- */
/* -- Static Variables need to be instantiated in CPP
      Check .h file for definition.                                */

QHash<int, HMVariable *> Project::m_hmvariable_store;
QHash<int, Unit *> Project::m_unit_store;
QHash<int, NamedObjectWithID *> Project::m_lookup_table;
QHash<int, ProjectInput *> Project::m_project_inputs_store;
QHash<int, HSC *> Project::m_HSC_store;

 Simulation * Project::m_simulation;

QDomElement Project::m_elConfig;
QDir * Project::m_ConfigPath;
QDir * Project::m_TmpPath;
RasterManager::RasterMeta * Project::m_RasterTemplateMeta;

/* --------------------------------------------------------------- */


Project::Project(QString sXMLConfig, QString sXMLOutput, QString sXMLLogFile)
    : NamedObjectWithID("TEMP", -1)
{
    QString temp = sXMLOutput;
    temp = sXMLLogFile;

    // Load the object model into memory:
    // Also do all the in between steps
    Load(sXMLConfig);

    // Run the actual simulations. This is a polymorhic virtual function.
    m_simulation->Run();

}

Project::~Project(){

    // Empty the survey store
    QHashIterator<int, HMVariable *> i(m_hmvariable_store);
    while (i.hasNext()) {
        i.next();
        delete i.value();
    }

    // Empty the survey store
    QHashIterator<int, Unit *> j(m_unit_store);
    while (j.hasNext()) {
        j.next();
        delete j.value();
    }

    // Empty the survey store
    QHashIterator<int, HSC *> k(m_HSC_store);
    while (k.hasNext()) {
        k.next();
        delete k.value();
    }

    // Empty the survey store
    QHashIterator<int, NamedObjectWithID *> l(m_lookup_table);
    while (l.hasNext()) {
        l.next();
        delete l.value();
    }

    // Empty the survey store
    QHashIterator<int, ProjectInput *> m(m_project_inputs_store);
    while (m.hasNext()) {
        m.next();
        delete m.value();
    }

    delete m_simulation;
}

void Project::Load(QString sXMLConfig)
{
    GCD::Project::XMLFileHander xConfig;

    QDir sXMLConfigDir = QFileInfo(sXMLConfig).absoluteDir();
    m_ConfigPath = &sXMLConfigDir;

    // Make a temporary folder for this simulation
    if (m_ConfigPath->mkdir("tmp")){
        QDir tmpPath = QDir(m_ConfigPath->absolutePath() + QDir::separator() + "tmp");
        m_TmpPath = new QDir(tmpPath);
    }
    else {
        throw std::runtime_error(std::string("Could not create \"tmp\" temporary at: \"" + m_ConfigPath->absolutePath().toStdString() + "\". Does it already exist?"));
    }

    xConfig.Load(sXMLConfig);

    m_elConfig = xConfig.Document()->documentElement();

    QDomElement elSimulation =  m_elConfig.firstChildElement("Simulations");

    // This is not ideal way to do this but we do it since it is the top level element
    SetName(elSimulation.firstChildElement("Title").text().toStdString().c_str());
    SetID(elSimulation.firstChildElement("SimulationID").text().toInt());

    if (elSimulation.isNull())
        throw std::runtime_error("The <Simulations> node is missing from the Configuration XML file.");
    else
    {
        // Populate our lookup table hashes
        LoadLookupTable();
        LoadHMVariables();
        LoadUnits();
        LoadHSCs();

        LoadProjectInputs();

        // Calculate the raster union and make rasters from CSV
        PrepareProjectInputs();

        int nSimulationID = elSimulation.firstChildElement("SimulationID").text().toInt();

        int nSimulationHSIID = elSimulation.firstChildElement("HSIID").text().toInt();
        int nSimulationFISID = elSimulation.firstChildElement("FISID").text().toInt();


        bool bHSIID, bFISID;

        QDomNodeList elConfigHSIs = m_elConfig.elementsByTagName("HSI");

        for(int n= 0; n < elConfigHSIs.length(); n++){
            QDomElement elHSI = elConfigHSIs.at(n).toElement();
            int nTestHSIID = elHSI.firstChildElement("HSIID").text().toInt();
            if (nSimulationHSIID == nTestHSIID)
                bHSIID = true;
        }

        if (bHSIID){
            m_simulation = new HSISimulation(&elSimulation);
        }
        else if(bFISID){
//             m_simulation = new FISSimulation (&elSimulation);
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

        int nProjectInputID = elProjectInput.firstChildElement("InputID").text().toInt();
        int nproject_type = GetInputType(sInputFilepath);

        switch(nproject_type) {

        case PROJECT_INPUT_RASTER :
            p_projectinput = new ProjectInputRaster(&elProjectInput);
            m_project_inputs_store.insert(n, p_projectinput);
            break;
        case PROJECT_INPUT_VECTOR :
            p_projectinput = new ProjectInputVector(&elProjectInput);
            m_project_inputs_store.insert(n, p_projectinput);
            break;
        case PROJECT_INPUT_CSV :
            p_projectinput = new ProjectInputCSV(&elProjectInput);
            m_project_inputs_store.insert(n, p_projectinput);
            break;
        case PROJECT_INPUT_UNDEFINED :
            throw std::runtime_error(std::string("ERROR: No valid input file detected ") + sInputFilepath.toStdString());
            break;

        }
    }
}

void Project::PrepareProjectInputs(){

    // First do the Rasters to find the union intersection
    // RasterMeta
    QHashIterator<int, ProjectInput *> rInputs(m_project_inputs_store);
    bool bFirst = true;
    while (rInputs.hasNext()) {
        rInputs.next();
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
    rInputs.toFront();
    // Next Call Prepare on Each Raster
    while (rInputs.hasNext()) {
        rInputs.next();
        rInputs.value()->Prepare();
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


}
