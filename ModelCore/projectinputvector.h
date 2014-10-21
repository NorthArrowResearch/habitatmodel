#ifndef PROJECTINPUTVECTOR_H
#define PROJECTINPUTVECTOR_H

#include <QString>
#include "projectinput.h"

namespace HabitatModel {

class ProjectInputVector : ProjectInput
{
public:
    ProjectInputVector(const char *sName, int nID, const char * psValueField);
    QString getValueField();

private:
    QString m_sValueField;
};

}

#endif // PROJECTINPUTVECTOR_H
