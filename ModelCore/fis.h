#ifndef FIS_H
#define FIS_H

#include <QString>
#include "namedobject.h"
#include "project.h"
#include "habitat_exception.h"

namespace HabitatModel{

class FIS : public NamedObjectWithID
{
public:
    FIS(QDomElement *elFIS);
    ~FIS();

private:

    QString m_sFISRuleFile;

};

}

#endif // FIS_H
