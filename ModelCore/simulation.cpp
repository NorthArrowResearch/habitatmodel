#include <QString>

#include "simulation.h"
#include "hsi.h"
#include "project.h"

namespace HabitatModel{

Simulation::Simulation(QDomElement * elSimulation)
    : NamedObjectWithID(elSimulation, "Title", "SimulationID")
{

    //    Example Node:
    //    <Simulations>
    //    <SimulationID>4</SimulationID>
    //    <HSIID>3</HSIID>
    //    <CreatedBy>James</CreatedBy>
    //    <CreatedOn>2014-10-17T14:03:00.676-06:00</CreatedOn>
    //    <IsQueuedToRun>true</IsQueuedToRun>
    //    <HSISourcePath>HSI Source Path</HSISourcePath>
    //    <Title>ChinookSpawner</Title>
    //    <Folder>C:\Users\A01674762\Desktop\ScratchWorkspace\HabitatSuitabilityModeling\JohnStockton\Simulations\ChinookSpawner</Folder>
    //    <AddIndividualOutput>true</AddIndividualOutput>
    //    </Simulations>

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
