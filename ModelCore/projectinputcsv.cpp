#include "projectinputcsv.h"
#include "projectinput.h"
#include "project.h"
#include "raster.h"

namespace HabitatModel{

ProjectInputCSV::ProjectInputCSV(QDomElement * elProjectInput)
    : ProjectInput(elProjectInput)
{
    /*
    <ProjectDataSources>
      <DataSourceID>1</DataSourceID>
      <DataSourceTypeID>61</DataSourceTypeID>
      <OriginalPath>Y:\Projects\HabitalModel\Data\2014_10_24 SampleCSVs\DEM_GRID_OJD03458-000536_2012VisitID_748_Delft3D_Results.csv</OriginalPath>
      <ProjectPath>Inputs\DEMCSV\DEMCSV.csv</ProjectPath>
      <XField>X</XField>
      <YField>Y</YField>
      <CreatedOn>2014-11-26T17:01:13.719-08:00</CreatedOn>
      <Title>DEM CSV</Title>
    </ProjectDataSources>
    */
    QString sXFieldName  = elProjectInput->firstChildElement("XField").text();
    QString sYFieldName  = elProjectInput->firstChildElement("YField").text();
    Init(sXFieldName, sYFieldName);

}

ProjectInputCSV::ProjectInputCSV( const ProjectInputCSV &source): ProjectInput(source)
{
    Init(source.GetXFieldName(), source.GetYFieldName());
}

void ProjectInputCSV::Init(QString sXFieldName,QString sYFieldName){
    m_ProjectInputType = PROJECT_INPUT_CSV;
    m_sXFieldName = sXFieldName;
    m_sYFieldName = sYFieldName;
    //NOte: m_sFieldName only gets set on prepare()
}

void ProjectInputCSV::Prepare(RasterManager::RasterMeta * TemplateRaster, QString NewPath){

    Project::GetOutputXML()->Log("Preparing CSV Input: " + GetName() , 3);

//    m_sFieldName  = elProjectInput->firstChildElement("FieldName").text();
    // TODO: build safe filename from csv input
    // simulationname-hsisimulation-inputid-fname.tif
//    QString sNewFileName = "InputCSV-" + getInputFileName() + ".tif";

//    m_sRasterInputFileName = Project::GetTmpPath()->absoluteFilePath(sNewFileName);

//    RasterManager::Raster::CSVtoRaster(getSourceFilePath().toStdString().c_str(),
//                                       m_sRasterInputFileName.toStdString().c_str(),
//                                       m_sXFieldName.toStdString().c_str(),
//                                       m_sYFieldName.toStdString().c_str(),
//                                       m_sFieldName.toStdString().c_str(),
//                                       TemplateRaster );
}


}
