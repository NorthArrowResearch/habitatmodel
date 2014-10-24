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

    // Get the habitat suitability value for the specified category
    double GetHSValue(int nCategory);

private:
    /**
     * @brief LoadCategories load all related
     * Categories using the XML config file
     */
    void LoadCategories();

    QHash<int, HSCCategory *> m_categories;

};

} // Habitat Model

#endif // HSCCATEGORICAL_H
