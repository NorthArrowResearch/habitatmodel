#include "hmvariable.h"

namespace HabitatModel{

HMVariable::HMVariable(const char * sName, int nID) : NamedObjectWithID(sName, nID)
{
}

HMVariable::HMVariable(const char *sName, int nID,
                       NamedObjectWithID * p_dimension,
                       NamedObjectWithID * p_category)
    : NamedObjectWithID(sName,nID)
{
    m_dimension = p_dimension;
    m_category = p_category;
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
