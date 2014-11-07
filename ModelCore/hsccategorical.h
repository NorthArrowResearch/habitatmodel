#ifndef HSCCATEGORICAL_H
#define HSCCATEGORICAL_H

#include "hsccategory.h"
#include "hsc.h"
#include <QHash>

class HSCCategory;

namespace HabitatModel {

class HSCCategorical : HSC
{
public:
    HSCCategorical(QDomElement * elHSC);
    ~HSCCategorical();

    inline int GetCategory() { return m_nCategoryValue;}
    inline int GetHSValue() { return m_fHSValue;}

    // Get the habitat suitability value for the specified category
    double GetHSValue(int nCategory);

private:
    /**
     * @brief LoadCategories load all related
     * Categories using the XML config file
     */
    void LoadCategories();

    QHash<int, HSCCategory *> m_categories;

    int m_nCategoryID; // this is the model configuration database ID
    int m_nCategoryValue; // this is the category value (i.e. vegetation class 1 = bare earth)
    double m_fHSValue; // Habitat suitability value

};

} // Habitat Model

#endif // HSCCATEGORICAL_H
