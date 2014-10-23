#include "projectinput.h"
#include "project.h"

namespace HabitatModel{

ProjectInput::ProjectInput(const char *sName, int nID)
    : NamedObjectWithID(sName, nID)
{

}

ProjectInput::ProjectInput(QDomElement * elProjectInput)
    : NamedObjectWithID(elProjectInput, "Title", "InputID")
{
    //<ProjectInputs>
    //<InputID>1</InputID<name />
    //<ProjectID>1</ProjectID>
    //<Title>MyPh</Title>
    //<SourceTypeID>60</SourceTypeID>
    //<CreatedOn>2014-10-17T13:43:59.18-06:00</CreatedOn>
    //<SourcePath>C:\Users\A01674762\Desktop\ScratchWorkspace\SetNullTest_DomainPath.tif</SourcePath>
    //<UnitID>6</UnitID>
    //<VariableID>1</VariableID>
    //</ProjectInputs>

    m_screated_on = elProjectInput->firstChildElement("CreatedOn").text();
    m_sourcefilepath = elProjectInput->firstChildElement("SourcePath").text();

    m_variable = Project::GetVariable(elProjectInput, "VariableID");
    m_source_type = Project::GetLookupTableItem(elProjectInput, "SourceTypeID");

    m_unit = Project::GetUnit(elProjectInput, "UnitID");

}

//Project * ProjectInput::getProject()
//{
////    return m_project;
//}

NamedObjectWithID * ProjectInput::getSourceType()
{
    return m_source_type;
}

QString ProjectInput::getCreatedOn()
{
    return m_screated_on;
}

QString ProjectInput::getSourceFilePath()
{
    return m_sourcefilepath;
}

HMVariable * ProjectInput::getVariable()
{
    return m_variable;
}

Unit * ProjectInput::getUnit()
{
    return m_unit;
}

}
