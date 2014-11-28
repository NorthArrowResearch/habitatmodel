#include "projectinputvector.h"

namespace HabitatModel {

ProjectInputVector::ProjectInputVector(const char *sName, int nID, const char * psValueField) :
    ProjectInput(sName, nID)
{
    QString sValueField = QString(psValueField);
    Init(sValueField);
}

void ProjectInputVector::operator =(ProjectInputVector &source)
{

}

ProjectInputVector::ProjectInputVector(ProjectInputVector &source)
{

}

ProjectInputVector::ProjectInputVector(QDomElement * elProjectInput)
    : ProjectInput(elProjectInput)
{

//    m_sXField = QString(psXField);
//    m_sYField = QString(psYField);
//    m_sValueField = QString(psValueField);


}

void ProjectInputVector::Init(QString sValueField){
     m_ProjectInputType = PROJECT_INPUT_VECTOR;
     m_sValueField = sValueField;
}

void ProjectInputVector::Prepare(RasterManager::RasterMeta * TemplateRaster, QString NewPath){

}

QString ProjectInputVector::getValueField()
{
    return m_sValueField;
}

}
