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

void HSISimulation::RunSimulation(){

    // Loop over the input store and do the union of all input rasters
    QHashIterator<int, ProjectInput *> i(Project::GetProjectInputIterator());
    bool bFirst = true;
    while (i.hasNext()) {
        i.next();
        if (dynamic_cast<ProjectInputRaster*>(i.value())){
            // Load the raster.
            RasterManager::RasterMeta erRasterInput (i.value()->getSourceFilePath().toStdString().c_str());
            // First time round set the bounds to the first raster we give it.
            if (bFirst){
                RasterManager::RasterMeta startingRect (&erRasterInput);
                m_RasterTemplateMeta = &startingRect;
                bFirst = false;
            }
            else {
                m_RasterTemplateMeta->Union(&erRasterInput);
            }
        }
    }

    // Loop over the input store again and call "prepare" on each input
    QHashIterator<int, ProjectInput *> j(Project::GetProjectInputIterator());
    while (j.hasNext()) {
        j.next();
        j.value()->Prepare();
    }

}

RasterManager::ExtentRectangle * HSISimulation::GetRasterExtents(){
    return m_RasterTemplateMeta;
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
