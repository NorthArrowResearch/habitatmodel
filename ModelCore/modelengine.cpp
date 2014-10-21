#include "modelengine.h"
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QXmlStreamWriter>

#include "xmlfilehander.h"
#include "simulation.h"
#include "hsi.h"

namespace HabitatModel{

ModelEngine::ModelEngine(QString sXMLConfig, QString sXMLOutput, QString sXMLLogFile)
{
    Load(sXMLConfig);
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

        // Now Create our HSI object if there is one.
        QDomElement elHSI = elConfig.firstChildElement("HSI");
        if (!elHSI.isNull() && elHSI.firstChildElement("HSIID").text().toInt() == theSimulation.GetID()){
            HSI simHSI(&elHSI, &elConfig);
        }
    }

}

}
