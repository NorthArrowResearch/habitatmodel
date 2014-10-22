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

NamedObjectWithID *ModelEngine::GetLookupTableValue(int nid)
{
    return m_lookup_table.value(nid);
}

QDomElement * ModelEngine::GetConfig()
{
    return &m_elConfig;
}

void LoadUnits(){

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
