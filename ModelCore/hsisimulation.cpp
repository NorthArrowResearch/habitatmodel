#include "hsisimulation.h"

#include <QDomElement>
#include <QHash>
#include "hsi.h"
#include "project.h"
#include "projectinput.h"
#include "rastermeta.h"


namespace HabitatModel{


HSISimulation::HSISimulation(QDomElement *elSimulation)
    : Simulation(elSimulation)
{

    // Now Create our HSI object if there is one.
    QDomElement elHSI = Project::GetConfig()->firstChildElement("HSI");
    m_hsiRef = new HSI(&elHSI);

    LoadHSCInputs();
}


void HSISimulation::LoadHSCInputs(){

    //    <SimulationHSCInputs>
    //    <ProjectInputID>2</ProjectInputID>
    //    <HSICurveID>3</HSICurveID>
    //    <SimulationID>5</SimulationID>
    //  </SimulationHSCInputs>

    QDomNodeList elHSCInputs = Project::GetConfig()->elementsByTagName("SimulationHSCInputs");

    for(int n= 0; n < elHSCInputs.length(); n++){
        QDomElement elHSCInput = elHSCInputs.at(n).toElement();

        int nHSICurveID = elHSCInput.firstChildElement("HSICurveID").text().toInt();
        int nProjectInputID = elHSCInput.firstChildElement("ProjectInputID").text().toInt();

        HSICurve * p_HSICurve = m_hsiRef->GetCurve(nHSICurveID);
        ProjectInput * p_ProjectInput;

        m_simulation_hsc_inputs.insert(n, new SimulationHSCInput(p_HSICurve, p_ProjectInput));
    }
}


}
