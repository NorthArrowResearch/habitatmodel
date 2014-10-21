#include <QString>

#include "simulation.h"


namespace HabitatModel{

Simulation::Simulation(const char * sName, int nID) : NamedObjectWithID(sName, nID)
{
//    QString qsFilePath = "blah.xml";
//    LoadXML(sName);
}

Simulation::Simulation(QDomElement * elSimulation, QDomElement * elConfig) : NamedObjectWithID(elSimulation, "Title", "SimulationID")
{

    // First set all the member variables according to what's in the XML
    m_sfolder = elSimulation->firstChildElement("Folder").text();
    m_shsi_source_path = elSimulation->firstChildElement("HSISourcePath").text();

    QString sQueued = elSimulation->firstChildElement("IsQueuedToRun").text();
    if (sQueued == "true" || sQueued == "1")
        m_bqueued = true;
    else
        m_bqueued = false;

    QString sAddIndOut = elSimulation->firstChildElement("AddIndividualOutput").text();
    if (sAddIndOut == "true" || sAddIndOut == "1")
        m_badd_individual_output = true;
    else
        m_badd_individual_output = false;

    m_screated_on = elSimulation->firstChildElement("CreatedOn").text();
    m_screated_by = elSimulation->firstChildElement("CreatedBy").text();


}




}
