#include "hsisimulation.h"

#include <QDomElement>
#include <QHash>
#include "hsi.h"
#include "project.h"
#include "projectinput.h"

#include "extentrectangle.h"

namespace HabitatModel{


HSISimulation::HSISimulation(QDomElement *elSimulation)
    : Simulation(elSimulation)
{

    // Now Create our HSI object if there is one.
    QDomElement elHSI = Project::GetConfig()->firstChildElement("HSI");
    m_hsiRef = new HSI(&elHSI);

    LoadHSCInputs();
}

void HSISimulation::RunSimulation(){

    // Loop over the input store and do the union of all input rasters
    QHashIterator<int, ProjectInput *> i(Project::GetProjectInputIterator());
    while (i.hasNext()) {
        i.next();
        if (dynamic_cast<ProjectInputRaster*>(i.value())){
            // this is where the extent builder stuff will live.
//            RasterManager::ExtentRectangle
        }
    }

    // Loop over the input store again and clal prepare on each input
    QHashIterator<int, ProjectInput *> j(Project::GetProjectInputIterator());
    while (j.hasNext()) {
        j.next();
        j.value()->Prepare();
    }

    /*
     *  loop over the input store
     *       if raster then builf union extent
     *
     * Loop over input store
     *     call input.prepare()
     *
     */
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
