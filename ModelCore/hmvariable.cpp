#include "hmvariable.h"
#include "modelengine.h"

namespace HabitatModel{

HMVariable::HMVariable(const char * sName, int nID) : NamedObjectWithID(sName, nID)
{
}

HMVariable::HMVariable(QDomElement * elHSC) : NamedObjectWithID(elHSC, "VariableName", "VariableID")
{
    m_dimension = ModelEngine::GetLookupTableItem(elHSC,"DimensionID");
    m_category = ModelEngine::GetLookupTableItem(elHSC,"DimensionID");
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
