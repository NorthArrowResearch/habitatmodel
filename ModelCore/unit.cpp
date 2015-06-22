#include "unit.h"
#include "project.h"

namespace HabitatModel{

Unit::Unit(const char * sName, int nID, QString sAbbrev, NamedObjectWithID * p_dimension)
    : NamedObjectWithID(sName, nID)
{
    s_abbreviation = sAbbrev;
    m_dimension = p_dimension;
}
Unit::Unit(QDomElement * elUnit) : NamedObjectWithID(elUnit, "Title", "UnitID")
{
    s_abbreviation = elUnit->firstChildElement("Abbreviation").text();
    m_dimension = Project::GetLookupTableItem(elUnit,"DimensionID");
}

QString Unit::getAbbreviation()
{
    return s_abbreviation;
}

NamedObjectWithID * Unit::getDimension()
{
    return m_dimension;
}

}
