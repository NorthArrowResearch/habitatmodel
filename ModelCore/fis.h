#ifndef FIS_H
#define FIS_H

#include <QString>
#include "namedobject.h"

namespace HabitatModel{

class FIS : public NamedObjectWithID
{
public:
    FIS(QDomElement *elFIS);
    ~FIS();
};

}

#endif // FIS_H
