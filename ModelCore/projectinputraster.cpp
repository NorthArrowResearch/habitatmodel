#include "projectinputraster.h"

namespace HabitatModel {



ProjectInputRaster::ProjectInputRaster(const char *sName, int nID) :
    ProjectInput(sName, nID)
{

}

void ProjectInputRaster::Prepare(){
    m_sRasterInputFileName = getSourceFilePath();
}

ProjectInputRaster::ProjectInputRaster(QDomElement * elProjectInput)
    : ProjectInput(elProjectInput)
{

//    m_sXField = QString(psXField);
//    m_sYField = QString(psYField);
//    m_sValueField = QString(psValueField);

}


}
