#include "hmvariable.h"
#include "project.h"

namespace HabitatModel{

HMVariable::HMVariable(const char * sName, int nID) : NamedObjectWithID(sName, nID)
{
}

HMVariable::HMVariable(QDomElement * elHSC) : NamedObjectWithID(elHSC, "VariableName", "VariableID")
{
    m_dimension = Project::GetLookupTableItem(elHSC,"DimensionID");
    m_category = Project::GetLookupTableItem(elHSC,"DimensionID");
}

NamedObjectWithID * HMVariable::getDimentsion()
{
    return m_dimension;
}

NamedObjectWithID * HMVariable::getCategory()
{
    return m_category;
}

}
