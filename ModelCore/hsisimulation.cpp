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

void HSISimulation::Run(){

    //Method of combination
    int nMethod = DetermineMethod();

    // Our final output Raster file name and path:
    QString sNewFileName = "OUTPUT-" + GetName() + ".tif";
    m_OutputRasterFileName = Project::GetTmpPath()->absoluteFilePath(sNewFileName);

    // Get and loop over all the simulationHSCInputs
    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);

    while (i.hasNext()) {
        i.next();
        // Here is the curve we want
        SimulationHSCInput * testa = i.value();
        HSICurve * testb = testa->GetHSICurve();
        HSC * pHSC = i.value()->GetHSICurve()->GetHSC();

        // Here is the corresponding input raster
        ProjectInput * pInput = i.value()->GetProjectInput();

        QString pProjectInputFile = pInput->getInputRasterFileName();

        QString sNewUtilizationFileName = "UT-" + pInput->getInputRasterFileName();
        pInput->setUtilizationRasterFileName( Project::GetTmpPath()->absoluteFilePath(sNewUtilizationFileName) );

        // Use: Project::GetRasterExtentMeta() for size.

        //We don't know what kind of HSC it is so we need to type-check the value that gets spit back

        // Then we do something like
        // for rows
            // for cols
//               psOutput[j] = HSCInflection.GetHSValue(psInput[j]);
        //WRite file.


        // Create utilization raster
        // gdal writeio (GDT_float64 blah blah blah)
    }

    // Combine Output Rasters using HSIMethodID in HSI

    // This might be done by opening a QHash of buffers, each responsible for a line of the
    // Utilization raster. Then pass all the buffers into a function and let that functions
    // Sort it out.

        // Use: Project::GetRasterExtentMeta() for size.f

    // Reset the iterator to the begginning and run it again, this time working on outputs
    i.toFront();
    while (i.hasNext()) {
        i.next();

    }

}

int HSISimulation::DetermineMethod(){
    QString sMethod = m_hsiRef->GetMethod()->GetName();

    if ( sMethod.compare("Product") == 0 ){ return HSI_PRODUCT; }
    else if ( sMethod.compare("Arithmetic Mean") == 0 ){ return HSI_ARITHMETIC_MEAN; }
    else if ( sMethod.compare("Geometric Mean") == 0 ){ return HSI_GEOMETRIC_MEAN; }
    else if ( sMethod.compare("Minimum") == 0 ){ return HSI_MINIMUM; }
    else if ( sMethod.compare("Weighted Mean") == 0 ){ return HSI_WEIGHTED_MEAN; }
    else {
        throw std::runtime_error("ERROR:  Could not determine Method for Raster combination in HSI Simulation");
    }
}

void HSISimulation::Clean(){

}

void HSISimulation::LoadHSCInputs(){

    //  <SimulationHSCInputs>
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
