#include "projectinputcsv.h"
#include "projectinput.h"
#include "project.h"
#include "raster.h"
#include "habitat_exception.h"

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

    if (sXFieldName.compare("") == 0){
        Project::ProjectError(MISSING_FIELD, "the X field was not specified");
    }
    else if (sYFieldName.compare("") ==0){
        Project::GetOutputXML()->Log("the Y field was not specified. Continuing as if X is an index field.", 3);
    }

    m_sXFieldName = sXFieldName;
    m_sYFieldName = sYFieldName;


    //NOte: m_sFieldName only gets set on prepare()
}

void ProjectInputCSV::Prepare(Simulation * pSimulation){

    Project::GetOutputXML()->Log("Preparing CSV Input: " + GetName() , 3);

    // We only need a raster if one is requested.
    if (pSimulation->HasOutputRaster()){
        QString sCSVFilePath = GetSourceFilePath();
        QString sFinalRasterPath = GetPreparedRasterFileName();

        // Make sure there's a directory and delete any duplicate files.
        Project::EnsureFile(sFinalRasterPath);

        // Rasterman doesn't support Qstring so we have to step everything down to char*
        const QByteArray qbCSVFilePath = sCSVFilePath.toLocal8Bit();
        const QByteArray qbFinalRaster = sFinalRasterPath.toLocal8Bit();

        const QByteArray qbXFieldName = GetXFieldName().toLocal8Bit();
        const QByteArray qbYFieldName = GetYFieldName().toLocal8Bit();
        const QByteArray qbDataFieldName = GetValueFieldName().toLocal8Bit();

        Project::GetOutputXML()->Log("Create Tif from CSV: " + sCSVFilePath , 3);

        RasterManager::Raster::CSVtoRaster(qbCSVFilePath.data(),
                                           qbFinalRaster.data(),
                                           qbXFieldName.data(),
                                           qbYFieldName.data(),
                                           qbDataFieldName.data(),
                                           pSimulation->GetRasterExtentMeta());

    }

}

/**
 * @brief ProjectInputCSV::CSVCellClean -- Helper function to clean CSV cells
 * @param value
 */
void ProjectInputCSV::CSVCellClean(QString & value){

    value = value.trimmed();
    value.remove(QChar('"'), Qt::CaseInsensitive);
    // Trim again in case there's whitespace inside the quotes
    value = value.trimmed();
}


}
