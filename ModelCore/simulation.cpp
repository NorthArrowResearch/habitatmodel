#include <QString>

#include "simulation.h"


namespace HabitatModel{

Simulation::Simulation(const char * sName, int nID) : NamedObjectWithID(sName, nID)
{
//    QString qsFilePath = "blah.xml";
//    LoadXML(sName);
}


//Simulation::Simulation(XMLDomElement);


void Simulation::run()
{

}

void Simulation::clean()
{


}

void Simulation::LoadXML(QString qsXMLFile)
{
   // GCD::
}

void Simulation::CreateModelFromXML()
{

}

}
