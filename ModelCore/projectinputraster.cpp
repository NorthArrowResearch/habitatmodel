#include "projectinputraster.h"

namespace HabitatModel {



ProjectInputRaster::ProjectInputRaster(const char *sName, int nID) :
    ProjectInput(sName, nID)
{

}

void ProjectInputRaster::Prepare(RasterManager::RasterMeta * TemplateRaster, QString NewPath){
    m_sRasterInputFileName = getSourceFilePath();

    //TODO: resize the raster to the input template then change the source path to the new one


}

ProjectInputRaster::ProjectInputRaster(QDomElement * elProjectInput)
    : ProjectInput(elProjectInput)
{

}


}
