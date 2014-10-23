#ifndef HSC_H
#define HSC_H

#include <QString>
#include <QHash>
#include "namedobject.h"

namespace HabitatModel{

class Unit;
class HMVariable;
class HSCCategory;
class HSCCoordinatePair;

class HSC : public NamedObjectWithID
{
public:

    /**
     * @brief HSC
     * @param elHSC dom element containing an HSC
     */
    HSC(QDomElement *elHSC);

    /**
     * @brief LoadCoordinatePairs load all related
     * coordinate pairs using the XML config
     * file
     */
    void LoadCoordinatePairs();

    /**
     * @brief LoadCategories load all related
     * Categories using the XML config file
     */
    void LoadCategories();
private:
    NamedObjectWithID * m_source;
    NamedObjectWithID * m_lifestage;
    NamedObjectWithID * m_species;

    int m_figure;
    QString m_location;

    HMVariable * m_variable;
    Unit * m_unit;

    // We have some stores of objects related to this
    // HSC that we need to look up
    QHash<int, HSCCategory *> m_categories;
    QHash<int, HSCCoordinatePair *> m_coordinate_pairs;


};


}

#endif // HSC_H
