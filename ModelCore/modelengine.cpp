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

void ModelEngine::Load(QString sXMLConfig)
{
    GCD::Project::XMLFileHander xConfig;

    xConfig.Load(sXMLConfig);

    QDomElement elSimulation = xConfig.Document()->documentElement().firstChildElement("Simulations");

    if (elSimulation.isNull())
        throw "The main Simulation node is missing from the Configuration XML file.";
    else
    {
        QString sName = elSimulation.firstChildElement("Title").text();
        QString nID = elSimulation.firstChildElement("SimulationID").text();

        Simulation theSimulation(&elSimulation);

        QString testname = theSimulation.GetName();
    }

}

}
