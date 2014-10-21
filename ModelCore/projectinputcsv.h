#ifndef PROJECTINPUTCSV_H
#define PROJECTINPUTCSV_H

#include <QString>
#include "projectinput.h"

namespace HabitatModel{

class ProjectInputCSV : ProjectInput
{
public:
    ProjectInputCSV(const char *sName, int nID, const char *psXField, const char *psYField, const char *psValueField);
    QString getXField();
    QString getYField();
    QString getValueField();

private:
    QString m_sXField;
    QString m_sYField;
    QString m_sValueField;

};

}

#endif // PROJECTINPUTCSV_H
