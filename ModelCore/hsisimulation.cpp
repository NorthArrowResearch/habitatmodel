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
    m_HasRasters = false;

    // Now Create our HSI object if there is one.
    QDomElement elHSI = Project::GetConfigDom()->firstChildElement("HSI");
    m_hsiRef = new HSI(&elHSI);

    QString sRawHSISourcePath = QDir::fromNativeSeparators(elSimulation->firstChildElement("HSISourcePath").text());
    m_HSISourcePath = Project::GetProjectRootPath()->filePath(Project::SanitizePath(sRawHSISourcePath));

    // Make a local copy of each data source as a local simulation object,
    // ready for preparation.
    LoadInputs();

    // Now, if this thing is a raster we need to add it to the ExtentRectangle
    // For this simulation
    AddRastersToExtents();

    // Now that all the inputs are loaded we know the extent of the laoded
    // Rasters and we can prepare the inputs.
    PrepareInputs();
}

void HSISimulation::AddRastersToExtents(){

    // No Rasters, no point
    if (!HasRasters())
        return;

    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);

    while (i.hasNext()) {
        i.next();
        // Here is the curve we want
        ProjectInput * pInput = i.value()->GetProjectInput();

        if ( dynamic_cast <ProjectInputRaster *> ( pInput )){

            Project::GetOutputXML()->Log("Adding Raster to extent: " + i.value()->GetProjectInput()->GetName() , 2);

            std::string sRasterPath = i.value()->GetProjectInput()->GetSourceFilePath().toStdString();
            RasterManager::RasterMeta * pRasterMeta = new RasterManager::RasterMeta(sRasterPath.c_str());
            RasterUnion(pRasterMeta);
            delete pRasterMeta;
        }
    }
}

void HSISimulation::Run(){

    //Method of combination
    uint nMethod = DetermineMethod();

    // Our final output Raster file name and path:
    QString sNewFileName = "OUTPUT-" + GetName() + ".tif";
//    m_OutputRasterFileName = Project::GetTmpPath()->absoluteFilePath(sNewFileName);

    // Get and loop over all the simulationHSCInputs
    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);

    while (i.hasNext()) {
        i.next();

        // Here is the curve we want
        SimulationHSCInput * testa = i.value();
        HSICurve * testb = testa->GetHSICurve();
        HSC * pHSC = i.value()->GetHSICurve()->GetHSC();

        // Here is the corresponding input raster
//        ProjectInput * pInput = i.value()->GetProjectInput();

//        QString pProjectInputFile = pInput->getInputRasterFileName();

//        QString sNewUtilizationFileName = "UT-" + pInput->getInputRasterFileName();
//        pInput->setUtilizationRasterFileName( Project::GetTmpPath()->absoluteFilePath(sNewUtilizationFileName) );

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
        Project::ProjectError(SEVERITY_ERROR, "Could not determine Method for Raster combination in HSI Simulation");
    }
}

void HSISimulation::Clean(){

}

// This function loads the HSC Inputs but it also creates copies of the
// Inputs for preparation down in PrepareInputs()
void HSISimulation::LoadInputs(){

    Project::GetOutputXML()->Log("Loading Inputs for HSI Simulation: " + GetName() , 2);

    QDomNodeList elHSCInputs = Project::GetConfigDom()->elementsByTagName("SimulationHSCInputs");

    for(int n= 0; n < elHSCInputs.length(); n++){
        QDomElement elHSCInput = elHSCInputs.at(n).toElement();

        // It's easier to look up the hsi curve here.
        int nHSICurveID = elHSCInput.firstChildElement("HSICurveID").text().toInt();
        HSICurve * pHSICurve = m_hsiRef->GetCurve(nHSICurveID);

        SimulationHSCInput * newHSCInput = new SimulationHSCInput(elHSCInput, pHSICurve);

        ProjectInput * pInput = newHSCInput->GetProjectInput();
        if ( dynamic_cast <ProjectInputRaster *> ( pInput ))
            m_HasRasters = true;

        m_simulation_hsc_inputs.insert(n, newHSCInput);

    }
}

void HSISimulation::PrepareInputs(){

    Project::GetOutputXML()->Log("Preparing inputs for HSI Simulation: " + GetName() , 2);

    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);
    while (i.hasNext()) {
        i.next();
        i.value()->GetProjectInput()->Prepare(GetRasterExtentMeta());
    }

}

HSISimulation::~HSISimulation(){
    // Empty HSC input store
    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);
    while (i.hasNext()) {
        i.next();
        delete i.value();
    }

}


}
