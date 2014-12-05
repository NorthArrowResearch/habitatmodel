#include "projectinput.h"
#include "project.h"

namespace HabitatModel{


ProjectInput::ProjectInput(QDomElement * elProjectInput)
    : NamedObjectWithID(elProjectInput, "Title", "DataSourceID")
{

    QString sCreatedOn = elProjectInput->firstChildElement("CreatedOn").text();
    QString sProjectPath = Project::SanitizePath(elProjectInput->firstChildElement("ProjectPath").text());
    QString sProjectPathDir = Project::GetProjectRootPath()->filePath(sProjectPath);

    NamedObjectWithID * pSourceType = Project::GetLookupTableItem(elProjectInput, "DataSourceTypeID");

    Init(sCreatedOn, sProjectPathDir, pSourceType, "", "");
}

ProjectInput::ProjectInput(const ProjectInput &source)
    : NamedObjectWithID(source.GetName(), source.GetID()) {

    Init(source.GetCreatedOn(),
         source.GetSourceFilePath(),
         source.GetSourceType(),
         source.GetPreparedRasterFileName(),
         source.GetHSOutputRasterFileName());
}


void ProjectInput::Init(QString sCreatedOn,
                        QString sProjectPath,
                        NamedObjectWithID * pSourceType,
                        QString sUtilizationRasterFileName,
                        QString sHSOutputFileName){

    m_screated_on = sCreatedOn;
    m_sourcefilepath = sProjectPath;
    m_source_type = pSourceType;
    m_preparedrasterfilename = sUtilizationRasterFileName;
    m_hsoutputfilepath = sHSOutputFileName;

}

}
