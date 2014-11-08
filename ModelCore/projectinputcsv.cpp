#include "projectinputcsv.h"
#include "projectinput.h"
#include "project.h"
#include "raster.h"

namespace HabitatModel{

ProjectInputCSV::ProjectInputCSV(QDomElement * elProjectInput)
    : ProjectInput(elProjectInput)
{

    //    <ProjectInputs>
    //      <InputID>2</InputID>
    //      <ProjectID>1</ProjectID>
    //      <Title>MinD0</Title>
    //      <SourceTypeID>61</SourceTypeID>
    //      <CreatedOn>2014-10-17T14:01:38.219-06:00</CreatedOn>
    //      <SourcePath>C:\Users\A01674762\Desktop\TrafficSchoolInfo.csv</SourcePath>
    //      <FieldName>depth</FieldName>
    //      <xField>x</xField>
    //      <yField>y</yField>
    //      <UnitID>15</UnitID>
    //      <VariableID>9</VariableID>
    //    </ProjectInputs>

    m_sXFieldName  = elProjectInput->firstChildElement("xField").text();
    m_sYFieldName  = elProjectInput->firstChildElement("yField").text();
    m_sFieldName  = elProjectInput->firstChildElement("FieldName").text();

}

void ProjectInputCSV::Prepare(){

    // TODO: build safe filename from csv input
    // simulationname-hsisimulation-inputid-fname.tif
    QString sNewFileName = "InputCSV-" + getInputFileName() + ".tif";

    m_sRasterInputFileName = Project::GetTmpPath()->absoluteFilePath(sNewFileName);

    RasterManager::Raster::CSVtoRaster(getSourceFilePath().toStdString().c_str(),
                                       m_sRasterInputFileName.toStdString().c_str(),
                                       m_sXFieldName.toStdString().c_str(),
                                       m_sYFieldName.toStdString().c_str(),
                                       m_sFieldName.toStdString().c_str(),
                                       Project::GetRasterExtentMeta() );
}


}
