#ifndef HMVARIABLE_H
#define HMVARIABLE_H

#include "namedobject.h"

namespace HabitatModel{

class HMVariable : NamedObjectWithID
{
public:
    HMVariable(const char *sName, int nID);
    HMVariable(const char *sName, int nID, NamedObjectWithID * p_dimension, NamedObjectWithID * p_category);
    NamedObjectWithID * getDimentsion();
    NamedObjectWithID * getCategory();

private:
    NamedObjectWithID * m_dimension;
    NamedObjectWithID * m_category;
};

}

#endif // HMVARIABLE_H
