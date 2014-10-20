#include "projectinput.h"


namespace HabitatModel{

ProjectInput::ProjectInput(const char * sName, int nID) : NamedObjectWithID(sName, nID)
{
}

Project ProjectInput::getProject()
{
    return * m_project;
}

NamedObjectWithID ProjectInput::getSourceType()
{
    return * m_source_type;
}

QString ProjectInput::getCreatedOn()
{
    return qd_created_on;
}

QString ProjectInput::getSourcePath()
{
    return s_sourcepath;
}

HMVariable ProjectInput::getVariable()
{
    return * m_variable;
}

Unit ProjectInput::getUnit()
{
    return * m_unit;
}

}
