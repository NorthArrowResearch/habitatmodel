#include "projectinputcsv.h"
#include "projectinput.h"
#include "project.h"

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

    m_sXField  = elProjectInput->firstChildElement("xField").text().toInt();
    m_sYField  = elProjectInput->firstChildElement("yField").text().toInt();
    m_FieldName  = elProjectInput->firstChildElement("FieldName").text().toInt();

    CreateRasterFromCSV(getSourceFilePath());

}


void ProjectInputCSV::CreateRasterFromCSV(QString sCSVFilePath){

//    Project::GetConfigPath();
//    sCSVFilePath;

    // Define a raster
    float fNoDataValue = (float) std::numeric_limits<float>::min();


}

void ProjectInputCSV::Prepare(){

}

QString ProjectInputCSV::getXField()
{
    return m_sXField;
}

QString ProjectInputCSV::getYField()
{
    return m_sYField;
}

QString ProjectInputCSV::getValueField()
{
    return m_FieldName;
}

}
