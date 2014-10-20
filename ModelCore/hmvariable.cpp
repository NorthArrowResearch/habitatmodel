#include "hmvariable.h"

namespace HabitatModel{

HMVariable::HMVariable()
{
}

NamedObjectWithID HMVariable::getDimentsion()
{
    return m_dimension;
}

NamedObjectWithID HMVariable::getCategory()
{
    return m_category;
}

}
