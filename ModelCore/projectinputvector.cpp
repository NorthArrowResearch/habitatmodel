#include "projectinputvector.h"

namespace HabitatModel {

ProjectInputVector::ProjectInputVector(const char *sName, int nID, const char * psValueField) :
    ProjectInput(sName, nID)
{
    m_sValueField = QString(psValueField);
}

ProjectInputVector::ProjectInputVector(QDomElement * elProjectInput)
    : ProjectInput(elProjectInput)
{

//    m_sXField = QString(psXField);
//    m_sYField = QString(psYField);
//    m_sValueField = QString(psValueField);

}

void ProjectInputVector::Prepare(){

}

QString ProjectInputVector::getValueField()
{
    return m_sValueField;
}

}
