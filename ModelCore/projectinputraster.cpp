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

void ProjectInputRaster::Prepare(RasterManager::RasterMeta * TemplateRasterMeta){

    QString sOriginalRaster = GetSourceFilePath();
    QString sFinalPath = PrepareForInputFile();

    Project::GetOutputXML()->LogDebug("Copying file' : " + sOriginalRaster + " to:  " + sFinalPath , 3);

    // Rasterman doesn't support Qstring so we have to step everything down to char*
    const QByteArray qbOriginalRaster = sOriginalRaster.toLocal8Bit();
    const QByteArray qbFinalRaster = sFinalPath.toLocal8Bit();

    RasterManager::Copy( qbOriginalRaster.data(),
                         qbFinalRaster.data(),
                         TemplateRasterMeta->GetCellWidth(),
                         TemplateRasterMeta->GetLeft(),
                         TemplateRasterMeta->GetTop(),
                         TemplateRasterMeta->GetRows(),
                         TemplateRasterMeta->GetCols());
}






}
