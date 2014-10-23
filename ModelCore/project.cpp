#include "project.h"
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
QHash<int, HMVariable *> Project::m_hmvariable_store;
QHash<int, Unit *> Project::m_unit_store;
QHash<int, NamedObjectWithID *> Project::m_lookup_table;
QHash<int, HSC *> Project::m_HSC_store;
QDomElement Project::m_elConfig;

Project::Project(QString sXMLConfig, QString sXMLOutput, QString sXMLLogFile)
    :NamedObjectWithID("TEMP",-1)
{
    QString temp = sXMLOutput;
    temp = sXMLLogFile;
    Load(sXMLConfig);
}

NamedObjectWithID * Project::GetLookupTableItem(int nlistid)
{
    return m_lookup_table.value(nlistid);
}

NamedObjectWithID * Project::GetLookupTableItem(QDomNode *elItem, QString sValIDName)
{
    int nValID = elItem->firstChildElement(sValIDName).text().toInt();
    return GetLookupTableItem(nValID);
}
NamedObjectWithID * Project::GetLookupTableItem(QDomElement *elItem, QString sValIDName)
{
    int nValID = elItem->firstChildElement(sValIDName).text().toInt();
    return GetLookupTableItem(nValID);
}

HMVariable * Project::GetVariable(int nid){
    return m_hmvariable_store.value(nid);
}

HMVariable * Project::GetVariable(QDomElement *elItem, QString sValIDName){
    int nVarID = elItem->firstChildElement(sValIDName).text().toInt();
    return GetVariable(nVarID);
}

Unit * Project::GetUnit(int nid){
    return m_unit_store.value(nid);
}

Unit * Project::GetUnit(QDomElement *elItem, QString sUnitName){
    int nVarID = elItem->firstChildElement(sUnitName).text().toInt();
    return GetUnit(nVarID);
}

HSC * Project::GetHSC(int nid){
    return m_HSC_store.value(nid);
}

HSC * Project::GetHSC(QDomElement *elItem, QString sHSCName){
    int nVarID = elItem->firstChildElement(sHSCName).text().toInt();
    return GetHSC(nVarID);
}


QDomElement * Project::GetConfig()
{
    return &m_elConfig;
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

    for(int n= 0; n < elUnits.length(); n++){
        QDomElement elUnit = elUnits.at(n).toElement();
        int nListItemID = elUnit.firstChildElement("UnitID").text().toInt();
        m_unit_store.insert(nListItemID, new Unit(&elUnit));
    }
}

void Project::LoadHMVariables(){
    QDomNodeList elvars = m_elConfig.elementsByTagName("Variables");

    for(int n= 0; n < elvars.length(); n++){
        QDomElement elvar = elvars.at(n).toElement();
        int nvarID = elvar.firstChildElement("VariableID").text().toInt();
        m_hmvariable_store.insert(nvarID, new HMVariable(&elvar));
    }
}

void Project::LoadHSCs(){
    QDomNodeList elHSCs = m_elConfig.elementsByTagName("HSC");

    for(int n= 0; n < elHSCs.length(); n++){
        QDomElement elHSC = elHSCs.at(n).toElement();
        int HSCID = elHSC.firstChildElement("VariableID").text().toInt();
        m_HSC_store.insert(HSCID, new HSC(&elHSC));
    }
}

void Project::Load(QString sXMLConfig)
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
            p_simulation = new HSISimulation (&elSimulation);
        }
//        else if(bFISID){
//            //             p_simulation = FISSimulation (&elSimulation);
//        }
        else{
            throw "No valid HSI or FIS nodes found in the config file.";
        }

    }

}

}
