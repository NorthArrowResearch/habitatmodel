#include "hsisimulation.h"

#include <QDomElement>
#include <QHash>
#include "hsi.h"
#include "project.h"
#include "projectinput.h"
#include "rastermanager_interface.h"


namespace HabitatModel{


HSISimulation::HSISimulation(QDomElement *elSimulation)
    : Simulation(elSimulation)
{
    m_HasRasters = false;

    QDomElement * elHSI = NULL;
    // Now Create our HSI object if there is one.
    int nSimulationHSIID = elSimulation->firstChildElement("HSIID").text().toInt();

    QDomNodeList elConfigHSIs = Project::GetConfigDom()->elementsByTagName("HSI");

    bool bHSIFound = false;
    for(int n= 0; n < elConfigHSIs.length(); n++){
        if (!bHSIFound){
            elHSI = new QDomElement(elConfigHSIs.at(n).toElement());
            int nTestHSIID = elHSI->firstChildElement("HSIID").text().toInt();
            if (nSimulationHSIID == nTestHSIID)
                bHSIFound = true;
        }
    }

    if (elHSI == NULL)
        Project::ProjectError(SEVERITY_ERROR, "Project is missing an HSI.");

    m_hsiRef = new HSI(elHSI);

    delete elHSI;

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
    QString sHSIOutputFile = GetHSISourcePath();

    // Get and loop over all the simulationHSCInputs, comparing them to
    // their curves along the way.
    QHashIterator<int, SimulationHSCInput *> i(m_simulation_hsc_inputs);

    while (i.hasNext()) {
        i.next();

        // Here is the curve we want
        SimulationHSCInput * pSimHSCInput = i.value();
        HSC * pHSC = i.value()->GetHSICurve()->GetHSC();

        // Here is the corresponding input raster
        ProjectInput * pInput = i.value()->GetProjectInput();

        // Pure virtual function will decide if it's a categorical
        // or coordinate pair HSC
        pHSC->ProcessRaster( pInput->GetUtilizationRasterFileName(), //rename to prepareedsomething
                             pInput->GetHSOutputRasterFileName(),
                             GetRasterExtentMeta());

    }

    // Combine Output Rasters using HSIMethodID in HSI
    Project::GetOutputXML()->Log("Combining all output rasters into one: " + GetHSISourcePath() , 2);

    const QByteArray sHSIOutputQB = GetHSISourcePath().toLocal8Bit();
    GDALDataset * pOutputDS = RasterManager::CreateOutputDS( sHSIOutputQB.data(), GetRasterExtentMeta());

    // Reset the iterator to the begginning and run it again, this time working on outputs
    i.toFront();
    while (i.hasNext()) {
        i.next();

//        // Rasterman doesn't use Qt so we need to step down to char *
//        const QByteArray sInputQB = sInput.toLocal8Bit();
//        GDALDataset * pInputDS = (GDALDataset*) GDALOpen( sInputQB.data(), GA_ReadOnly);

//        //allocate memory for reading from DEM and writing to hillshade
//        QHash<int, HSCCoordinatePair *> m_coordinate_pairs;
//        unsigned char *pReadBuffer = (unsigned char*) CPLMalloc(sizeof(int)*sOutputRasterMeta->GetCols());

//        //loop through each DEM cell and do the hillshade calculation, do not loop through edge cells
//        for (int i=1; i < sOutputRasterMeta->GetRows() - 1; i++)
//        {
//            //assign no data for first and last positions in the row
//            pReadBuffer[0] = sOutputRasterMeta->GetNoDataValue(), pReadBuffer[ sOutputRasterMeta->GetCols() - 1 ] = sOutputRasterMeta->GetNoDataValue();

//            for (int j=1; j < sOutputRasterMeta->GetCols() - 1; j++)
//            {
//                pReadBuffer[j] =  CombineUsingMethod();

//                Select Case aSim.HSIRow.HSIMethodID
//                    Case 44 ' Arithmetic Mean
//                        fHSIValue += aValueRow.HSCValue
//                    Case 45 ' Geometric(Mean)
//                        fHSIValue *= aValueRow.HSCValue
//                    Case 47 ' Minimum
//                        If nCount = 0 OrElse fHSIValue > aValueRow.HSCValue Then
//                            fHSIValue = aValueRow.HSCValue
//                        End If
//                    Case 46 ' Product
//                        fHSIValue *= aValueRow.HSCValue
//                    Case 48 ' Weighted (Mean)
//                        fHSIValue += aValueRow.HSCValue * aValueRow.HSICurvesRow.Weight
//                    Case Else
//                        Dim ex As New Exception("Unrecognized HSI method")
//                        ex.Data.Add("HSI Method ID", aSim.HSIRow.HSIMethodID.ToString)
//                        Throw ex
//                End Select


//            }
//            pOutputDS->GetRasterBand(1)->RasterIO(GF_Write,0,i,
//                                                  sOutputRasterMeta->GetCols(),1,
//                                                  pReadBuffer,
//                                                  sOutputRasterMeta->GetCols(),1,
//                                                  sOutputRasterMeta->GetGDALDataType(),
//                                                  0,0 );
//        }
        //close datasets
//        GDALClose(pInputDS);


//        CPLFree(pReadBuffer);
//        pReadBuffer = NULL;

    }

    GDALClose(pOutputDS);


}


int HSISimulation::DetermineMethod(){
    if (m_hsiRef->GetMethod() == NULL)
        Project::ProjectError(SEVERITY_ERROR, "This HSI has no method.");

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
