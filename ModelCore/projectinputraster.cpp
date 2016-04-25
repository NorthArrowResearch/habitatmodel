#include "projectinputraster.h"
#include "project.h"
#include "habitat_interface.h"
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

void ProjectInputRaster::Prepare(Simulation * pSimulation){

    QString sOriginalRaster = GetSourceFilePath();

    QString sFinalPath = GetPreparedRasterFileName();

    // Make sure there's a directory and delete any duplicate files.
    Project::EnsureFile(sFinalPath);

    Project::GetOutputXML()->LogDebug("Copying file' : " + sOriginalRaster + " to:  " + sFinalPath , 3);

    RasterManager::RasterMeta * TemplateRasterMeta = pSimulation->GetRasterExtentMeta();

    // Rasterman doesn't support Qstring so we have to step everything down to char*
    const QByteArray qbOriginalRaster = sOriginalRaster.toLocal8Bit();
    const QByteArray qbFinalRaster = sFinalPath.toLocal8Bit();

    char* sErr = new char[ERRBUFFERSIZE];


    RasterManager::Copy( qbOriginalRaster.data(),
                         qbFinalRaster.data(),
                         TemplateRasterMeta->GetCellWidth(),
                         TemplateRasterMeta->GetLeft(),
                         TemplateRasterMeta->GetTop(),
                         TemplateRasterMeta->GetRows(),
                         TemplateRasterMeta->GetCols(), sErr);
    delete [] sErr;
}






}
