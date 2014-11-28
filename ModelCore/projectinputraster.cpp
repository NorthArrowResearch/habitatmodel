#include "projectinputraster.h"

namespace HabitatModel {



ProjectInputRaster::ProjectInputRaster(const char *sName, int nID) :
    ProjectInput(sName, nID)
{
    Init();
}

void ProjectInputRaster::operator =(ProjectInputRaster &source)
{

}

ProjectInputRaster::ProjectInputRaster(ProjectInputRaster &source)
{

}

ProjectInputRaster::ProjectInputRaster(QDomElement * elProjectInput)
    : ProjectInput(elProjectInput)
{
    Init();
}

void ProjectInputRaster::Init(){
    m_ProjectInputType = PROJECT_INPUT_RASTER;
}

void ProjectInputRaster::Prepare(RasterManager::RasterMeta * TemplateRaster, QString NewPath){
    m_sRasterInputFileName = getSourceFilePath();

    // Delete and recopy it
//    if QFile::exists("/home/user/dst.txt")
//        QFile::remove("/home/user/dst.txt");

//    QFile::copy("/home/user/src.txt", "/home/user/dst.txt");

    //TODO: resize the raster to the input template then change the source path to the new one
}






}
