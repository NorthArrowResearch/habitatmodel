#include "modelengine.h"
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QXmlStreamWriter>

#include "xmlfilehander.h"
#include "simulation.h"

namespace HabitatModel{

ModelEngine::ModelEngine(QString sXMLConfig, QString sXMLOutput, QString sXMLLogFile)
{
    Load(sXMLConfig);
}

HMVariable *ModelEngine::GetVariable(int nid)
{
    return m_hmvariable_store.find(nid).value();
}

Unit *ModelEngine::GetUnit(int nid)
{
    return m_unit_store.find(nid).value();
}

void ModelEngine::Load(QString sXMLConfig)
{
    GCD::Project::XMLFileHander xConfig;

    xConfig.Load(sXMLConfig);

    QDomElement elConfig = xConfig.Document()->documentElement();

    QDomElement elSimulation = elConfig.firstChildElement("Simulations");
    if (elSimulation.isNull())
        throw "The main Simulation node is missing from the Configuration XML file.";
    else
    {
        // Create our simulation object
        Simulation theSimulation(&elSimulation, &elConfig);

    }

}

}
