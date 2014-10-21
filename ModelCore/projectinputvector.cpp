#include "projectinputvector.h"

namespace HabitatModel {

ProjectInputVector::ProjectInputVector(const char *sName, int nID, const char * psValueField) :
    ProjectInput(sName, nID)
{
    m_sValueField = QString(psValueField);
}

QString ProjectInputVector::getValueField()
{
    return m_sValueField;
}

}
