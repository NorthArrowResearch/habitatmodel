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
    m_ProjectInputType = PROJECT_INPUT_RASTER;
    // Here's where we need to run Simulation::RasterUnion

}

void ProjectInputRaster::Prepare(RasterManager::RasterMeta * TemplateRaster, QString sPreparedRasterPath){
//    m_sourcefilepath = getSourceFilePath();

//    RasterManager::Copy()
    // Delete and recopy it
//    if QFile::exists("/home/user/dst.txt")
//        QFile::remove("/home/user/dst.txt");

//    QFile::copy("/home/user/src.txt", "/home/user/dst.txt");

    //TODO: resize the raster to the input template then change the source path to the new one
}






}
