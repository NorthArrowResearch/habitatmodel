#include "projectinput.h"
#include "project.h"

namespace HabitatModel{

ProjectInput::ProjectInput(const char *sName, int nID)
    : NamedObjectWithID(sName, nID)
{

}
\

ProjectInput::ProjectInput(QDomElement * elProjectInput)
    : NamedObjectWithID(elProjectInput, "Title", "DataSourceID")
{

    QString sCreatedOn = elProjectInput->firstChildElement("CreatedOn").text();
    QString sProjectPath = elProjectInput->firstChildElement("ProjectPath").text();

    HMVariable * pVariable = Project::GetVariable(elProjectInput, "VariableID");
    NamedObjectWithID * pSourceType = Project::GetLookupTableItem(elProjectInput, "DataSourceTypeID");

    Unit * pUnit = Project::GetUnit(elProjectInput, "UnitID");

    Init(sCreatedOn, sProjectPath, pVariable, pSourceType, pUnit);
}

ProjectInput::ProjectInput(ProjectInput &source)
    : NamedObjectWithID(source.GetName(), source.GetID()) {
    Init(source.getCreatedOn(),
         source.getSourceFilePath(),
         source.getVariable(),
         source.getSourceType(),
         source.getUnit());
}

void ProjectInput::operator=(ProjectInput &source)
{
    Init(source.getCreatedOn(),
         source.getSourceFilePath(),
         source.getVariable(),
         source.getSourceType(),
         source.getUnit());
}


void ProjectInput::Init(QString sCreatedOn, QString sProjectPath, HMVariable * pVariable,
                        NamedObjectWithID * pSourceType, Unit * pUnit){

    m_screated_on = sCreatedOn;
    m_sourcefilepath = sProjectPath;

    m_variable = pVariable;
    m_source_type = pSourceType;

    m_unit = pUnit;

}

}
