#include "projectinputcsv.h"
#include "projectinput.h"

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

//    m_sXField = QString(psXField);
//    m_sYField = QString(psYField);
//    m_FieldName = QString(psValueField);

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
