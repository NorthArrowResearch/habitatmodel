#include "unit.h"
#include "modelengine.h"

namespace HabitatModel{

Unit::Unit(const char * sName, int nID, QString sAbbrev, NamedObjectWithID * p_dimension) : NamedObjectWithID(sName, nID)
{

}
Unit::Unit(QDomElement * elUnit) : NamedObjectWithID(elUnit, "Title", "UnitID")
{
    s_abbreviation = elUnit->firstChildElement("Abbreviation").text();
    m_dimension = ModelEngine::GetLookupTableItem(elUnit,"DimensionID");
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
