#ifndef HMVARIABLE_H
#define HMVARIABLE_H

#include "namedobject.h"

namespace HabitatModel{

class HMVariable : NamedObjectWithID
{
public:
    HMVariable();
    NamedObjectWithID getDimentsion();
    NamedObjectWithID getCategory();

private:
    NamedObjectWithID * m_dimension;
    NamedObjectWithID * m_category;
};

}

#endif // HMVARIABLE_H
