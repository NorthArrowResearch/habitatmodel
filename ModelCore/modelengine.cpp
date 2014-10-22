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

namespace HabitatModel{

/* -- Static Variables need to be instantiated in CPP */
// Check .h file for definition.
QHash<int, HMVariable *> ModelEngine::m_hmvariable_store;
QHash<int, Unit *> ModelEngine::m_unit_store;
QHash<int, NamedObjectWithID *> ModelEngine::m_lookup_table;
QDomElement ModelEngine::m_elConfig;

ModelEngine::ModelEngine(QString sXMLConfig, QString sXMLOutput, QString sXMLLogFile)
{
    QString temp = sXMLOutput;
    temp = sXMLLogFile;
    Load(sXMLConfig);
}

HMVariable * ModelEngine::GetVariable(int nid)
{
    // TODO: check the find returns something.
    return m_hmvariable_store.value(nid);
}

Unit * ModelEngine::GetUnit(int nid)
{
    return m_unit_store.value(nid);
}

NamedObjectWithID * ModelEngine::GetLookupTable(int nlistid)
{
    return m_lookup_table.value(nlistid);
}


QDomElement * ModelEngine::GetConfig()
{
    return &m_elConfig;
}

void ModelEngine::LoadUnits(){

    QDomNodeList elUnits = m_elConfig.elementsByTagName("Units");

    for(int n= 0; n < elUnits.length(); n++){

        QDomNode elUnit = elUnits.at(n);
        int nListItemID = elUnit.firstChildElement("UnitID").text().toInt();
        QString sname = elUnit.firstChildElement("Title").text();
        QString sabbrev = elUnit.firstChildElement("Abbreviation").text();
        int ndimensionid = elUnit.firstChildElement("DimensionID").text().toInt();
        m_unit_store.insert(nListItemID, new Unit(sname.toStdString().c_str(), nListItemID, sabbrev, m_lookup_table.value(ndimensionid)));

    }

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

void ModelEngine::LoadHMVariables(){
    QDomNodeList elvars = m_elConfig.elementsByTagName("Variables");

    for(int n= 0; n < elvars.length(); n++){
        QDomNode elvar = elvars.at(n);
        int nvarID = elvar.firstChildElement("VariableID").text().toInt();
        QString sname = elvar.firstChildElement("VariableName").text();
        int ncatid = elvar.firstChildElement("CategoryID").text().toInt();
        int ndimensionid = elvar.firstChildElement("DimensionID").text().toInt();

        m_hmvariable_store.insert(nvarID, new HMVariable(sname.toStdString().c_str(), nvarID, m_lookup_table.value(ndimensionid), m_lookup_table.value(ncatid)));
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
