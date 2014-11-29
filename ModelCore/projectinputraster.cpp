#include "projectinputraster.h"
#include "project.h"
#include "rastermanager_interface.h"

namespace HabitatModel {


ProjectInputRaster::ProjectInputRaster(const ProjectInputRaster &source): ProjectInput(source)
{
    Init();
}

ProjectInputRaster::ProjectInputRaster(QDomElement * elProjectInput)
    : ProjectInput(elProjectInput)
{
    Init();
}

void ProjectInputRaster::Init(){


}

void ProjectInputRaster::Prepare(RasterManager::RasterMeta * TemplateRaster, QString sPreparedRasterPath){

    QFileInfo sFileInfo(GetSourceFilePath());

    std::string sOriginalRaster = GetSourceFilePath().toStdString();

    QString sFileName = sFileInfo.fileName();
    QString sDir = sFileInfo.dir().dirName();

    QFileInfo sNewFilePath(QDir(sPreparedRasterPath).filePath(sDir + QDir::separator() + sFileName));
    QDir sNewDir = QDir(sNewFilePath.absolutePath());

    Project::GetOutputXML()->Log("MAking : " + sNewDir.absolutePath() , 2);
    // Make a path if we don't have one already.
    if (!sNewDir.exists()){
      sNewDir.mkpath(".");
    }

    // Delete the file if it already exists.
   if (sNewFilePath.exists())
        QFile::remove(sNewFilePath.absoluteFilePath());

    std::string finalPath = sNewFilePath.absoluteFilePath().toStdString();

    RasterManager::Copy( sOriginalRaster.c_str(),
                         finalPath.c_str(),
                         TemplateRaster->GetCellWidth(),
                         TemplateRaster->GetLeft(),
                         TemplateRaster->GetTop(),
                         TemplateRaster->GetRows(),
                         TemplateRaster->GetCols());

}






}
