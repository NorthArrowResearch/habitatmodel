#include "projectinput.h"
#include "project.h"

namespace HabitatModel{


ProjectInput::ProjectInput(QDomElement * elProjectInput)
    : NamedObjectWithID(elProjectInput, "Title", "DataSourceID")
{

    QString sCreatedOn = elProjectInput->firstChildElement("CreatedOn").text();
    QString sProjectPath = Project::SanitizePath(elProjectInput->firstChildElement("ProjectPath").text());
    QString sProjectPathDir = Project::GetProjectRootPath()->filePath(sProjectPath);

    HMVariable * pVariable = Project::GetVariable(elProjectInput, "VariableID");
    NamedObjectWithID * pSourceType = Project::GetLookupTableItem(elProjectInput, "DataSourceTypeID");

    Unit * pUnit = Project::GetUnit(elProjectInput, "UnitID");

    Init(sCreatedOn, sProjectPathDir, pVariable, pSourceType, pUnit, "", "");
}

ProjectInput::ProjectInput(const ProjectInput &source)
    : NamedObjectWithID(source.GetName(), source.GetID()) {

    Init(source.GetCreatedOn(),
         source.GetSourceFilePath(),
         source.GetVariable(),
         source.GetSourceType(),
         source.GetUnit(),
         source.GetPreparedRasterFileName(),
         source.GetHSOutputRasterFileName());
}


void ProjectInput::Init(QString sCreatedOn, QString sProjectPath, HMVariable * pVariable,
                        NamedObjectWithID * pSourceType, Unit * pUnit, QString sUtilizationRasterFileName, QString sHSOutputFileName){

    m_screated_on = sCreatedOn;
    m_sourcefilepath = sProjectPath;
    m_variable = pVariable;
    m_source_type = pSourceType;
    m_preparedrasterfilename = sUtilizationRasterFileName;
    m_hsoutputfilepath = sHSOutputFileName;
    m_unit = pUnit;

}

}
