#ifndef HSCCATEGORY_H
#define HSCCATEGORY_H

#include <QDomElement>

namespace HabitatModel{

class HSCCategory
{
public:
    HSCCategory(QDomElement *elCategory);

    inline int GetCategory() { return m_nCategoryValue;}
    inline int GetHSValue() { return m_fHSVal;}

private:

    int m_nCategoryID; // this is the model configuration database ID
    int m_nCategoryValue; // this is the category value (i.e. vegetation class 1 = bare earth)
    double m_fHSVal; // Habitat suitability value

};

}

#endif // HSCCATEGORY_H
