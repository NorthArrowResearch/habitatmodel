#ifndef HMVARIABLE_H
#define HMVARIABLE_H

#include "namedobject.h"

namespace HabitatModel{

class HMVariable : NamedObjectWithID
{
public:

    /**
     * @brief HMVariable constructor takes a dom element
     * @param elHSC dom element
     */
    HMVariable(QDomElement *elHSC);

    /**
     * @brief getDimentsion
     * @return the dimension named object
     */
    NamedObjectWithID * getDimentsion();

    /**
     * @brief getCategory
     * @return the category named object
     */
    NamedObjectWithID * getCategory();

private:
    NamedObjectWithID * m_dimension;
    NamedObjectWithID * m_category;
};

}

#endif // HMVARIABLE_H
