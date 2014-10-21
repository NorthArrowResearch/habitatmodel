#include "unit.h"

namespace HabitatModel{

Unit::Unit(const char * sName, int nID) : NamedObjectWithID(sName, nID)
{

}

QString Unit::getAbbreviation()
{
    return s_abbreviation;
}

NamedObjectWithID Unit::getDimension()
{
    return * m_dimension;
}

}
