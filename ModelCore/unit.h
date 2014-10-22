#ifndef UNIT_H
#define UNIT_H

#include <QString>
#include "namedobject.h"

namespace HabitatModel{

class Unit : NamedObjectWithID
{
public:
    Unit(const char *sName, int nID, QString sAbbrev, NamedObjectWithID *p_dimension);
    QString getAbbreviation();
    NamedObjectWithID * getDimension();
private:
    NamedObjectWithID * m_dimension;
    QString s_abbreviation;
};

}



#endif // UNIT_H
