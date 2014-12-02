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

    Init(sCreatedOn, sProjectPathDir, pVariable, pSourceType, pUnit, "");
}

ProjectInput::ProjectInput(const ProjectInput &source)
    : NamedObjectWithID(source.GetName(), source.GetID()) {

    Init(source.GetCreatedOn(),
         source.GetSourceFilePath(),
         source.GetVariable(),
         source.GetSourceType(),
         source.GetUnit(),
         source.GetUtilizationRasterFileName());
}


QString ProjectInput::PrepareForInputFile(){

    QFileInfo sNewFilePath(GetUtilizationRasterFileName());
    QDir sNewDir = QDir(sNewFilePath.absolutePath());

    // Make a path if we don't have one already.
    if (!sNewDir.exists()){
      Project::GetOutputXML()->LogDebug("Dir Doesn't exist. Making' : " + sNewDir.absolutePath() , 3);
      sNewDir.mkpath(".");
    }

    // Delete the file if it already exists.
   if (sNewFilePath.exists())
        QFile::remove(sNewFilePath.absoluteFilePath());

   return sNewFilePath.absoluteFilePath();

}

void ProjectInput::Init(QString sCreatedOn, QString sProjectPath, HMVariable * pVariable,
                        NamedObjectWithID * pSourceType, Unit * pUnit, QString sUtilizationRasterFileName){

    m_screated_on = sCreatedOn;
    //<ProjectPath>Inputs\DEM\DEM.tif</ProjectPath>
    m_sourcefilepath = sProjectPath;

    m_variable = pVariable;
    m_source_type = pSourceType;
    m_sUtilizationFileName = sUtilizationRasterFileName;
    m_unit = pUnit;

}

}
