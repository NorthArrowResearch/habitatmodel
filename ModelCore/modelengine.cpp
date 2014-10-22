#include "modelengine.h"
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QHash>

#include "xmlfilehander.h"
#include "simulation.h"
#include "hsisimulation.h"
#include "hmvariable.h"
#include "unit.h"
#include "hsc.h"
#include "hmvariable.h"

namespace HabitatModel{

/* -- Static Variables need to be instantiated in CPP */
// Check .h file for definition.
QHash<int, HMVariable *> ModelEngine::m_hmvariable_store;
QHash<int, Unit *> ModelEngine::m_unit_store;
QHash<int, NamedObjectWithID *> ModelEngine::m_lookup_table;
QHash<int, HSC *> ModelEngine::m_HSC_store;
QDomElement ModelEngine::m_elConfig;

ModelEngine::ModelEngine(QString sXMLConfig, QString sXMLOutput, QString sXMLLogFile)
{
    QString temp = sXMLOutput;
    temp = sXMLLogFile;
    Load(sXMLConfig);
}

NamedObjectWithID * ModelEngine::GetLookupTableItem(int nlistid)
{
    return m_lookup_table.value(nlistid);
}

NamedObjectWithID * ModelEngine::GetLookupTableItem(QDomNode *elItem, QString sValIDName)
{
    int nValID = elItem->firstChildElement(sValIDName).text().toInt();
    return GetLookupTableItem(nValID);
}
NamedObjectWithID * ModelEngine::GetLookupTableItem(QDomElement *elItem, QString sValIDName)
{
    int nValID = elItem->firstChildElement(sValIDName).text().toInt();
    return GetLookupTableItem(nValID);
}

HMVariable * ModelEngine::GetVariable(int nid){
    return m_hmvariable_store.value(nid);
}

HMVariable * ModelEngine::GetVariable(QDomElement *elItem, QString sValIDName){
    int nVarID = elItem->firstChildElement(sValIDName).text().toInt();
    return GetVariable(nVarID);
}

Unit * ModelEngine::GetUnit(int nid){
    return m_unit_store.value(nid);
}

Unit * ModelEngine::GetUnit(QDomElement *elItem, QString sUnitName){
    int nVarID = elItem->firstChildElement(sUnitName).text().toInt();
    return GetUnit(nVarID);
}

HSC * ModelEngine::GetHSC(int nid){
    return m_HSC_store.value(nid);
}

HSC * ModelEngine::GetHSC(QDomElement *elItem, QString sHSCName){
    int nVarID = elItem->firstChildElement(sHSCName).text().toInt();
    return GetHSC(nVarID);
}


QDomElement * ModelEngine::GetConfig()
{
    return &m_elConfig;
}

void ModelEngine::LoadLookupTable(){
    QDomNodeList elListItems = m_elConfig.elementsByTagName("LookupListItems");

    for(int n= 0; n < elListItems.length(); n++){
        QDomNode elListItem = elListItems.at(n);
        int nListItemID = elListItem.firstChildElement("ItemID").text().toInt();
        QString sname = elListItem.firstChildElement("ItemName").text();
        m_lookup_table.insert(nListItemID, new NamedObjectWithID(sname.toStdString().c_str(), nListItemID));
    }
}

void ModelEngine::LoadUnits(){
    QDomNodeList elUnits = m_elConfig.elementsByTagName("Units");

    for(int n= 0; n < elUnits.length(); n++){
        QDomElement elUnit = elUnits.at(n).toElement();
        int nListItemID = elUnit.firstChildElement("UnitID").text().toInt();
        m_unit_store.insert(nListItemID, new Unit(&elUnit));
    }
}

void ModelEngine::LoadHMVariables(){
    QDomNodeList elvars = m_elConfig.elementsByTagName("Variables");

    for(int n= 0; n < elvars.length(); n++){
        QDomElement elvar = elvars.at(n).toElement();
        int nvarID = elvar.firstChildElement("VariableID").text().toInt();
        m_hmvariable_store.insert(nvarID, new HMVariable(&elvar));
    }
}

void ModelEngine::LoadHSCs(){
    QDomNodeList elHSCs = m_elConfig.elementsByTagName("HSC");

    for(int n= 0; n < elHSCs.length(); n++){
        QDomElement elHSC = elHSCs.at(n).toElement();
        int HSCID = elHSC.firstChildElement("VariableID").text().toInt();
        m_HSC_store.insert(HSCID, new HSC(&elHSC));
    }
}

void ModelEngine::Load(QString sXMLConfig)
{
    GCD::Project::XMLFileHander xConfig;

    xConfig.Load(sXMLConfig);

    m_elConfig = xConfig.Document()->documentElement();

    QDomElement elSimulation =  m_elConfig.firstChildElement("Simulations");
    if (elSimulation.isNull())
        throw "The main Simulation node is missing from the Configuration XML file.";
    else
    {
        Simulation * p_simulation;

        // Populate our lookup table hashes
        LoadLookupTable();
        LoadHMVariables();
        LoadUnits();
        LoadHSCs();

        int nHSIID = elSimulation.firstChildElement("HSIID").text().toInt();
        int nFISID = elSimulation.firstChildElement("FISID").text().toInt();

        if (nHSIID == m_elConfig.firstChildElement("HSI").firstChildElement("HSIID").text().toInt()){
             p_simulation = new HSISimulation (&elSimulation);
        }
        else if(nFISID == m_elConfig.firstChildElement("HSI").firstChildElement("HSIID").text().toInt()){
//             p_simulation = FISSimulation (&elSimulation);
        }
        else{
            throw "No valid HSI or FIS nodes found in the config file.";
        }

    }

}

}
