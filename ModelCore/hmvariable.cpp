#include "hmvariable.h"

namespace HabitatModel{

HMVariable::HMVariable(const char * sName, int nID) : NamedObjectWithID(sName, nID)
{
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
