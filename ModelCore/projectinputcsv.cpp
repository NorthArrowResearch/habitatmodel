#include "projectinputcsv.h"

namespace HabitatModel{

ProjectInputCSV::ProjectInputCSV(const char *sName, int nID, const char * psXField, const char * psYField, const char * psValueField)
    : ProjectInput(sName, nID)
{
    m_sXField = QString(psXField);
    m_sYField = QString(psYField);
    m_sValueField = QString(psValueField);
}

QString ProjectInputCSV::getXField()
{
    return m_sXField;
}

QString ProjectInputCSV::getYField()
{
    return m_sYField;
}

QString ProjectInputCSV::getValueField()
{
    return m_sValueField;
}

}
