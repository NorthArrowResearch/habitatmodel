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
    HSC(const char *sName, int nID);
private:
    NamedObjectWithID * m_source;
    NamedObjectWithID * m_lifestage;
    NamedObjectWithID * m_species;

    int figure;
    QString location;

    QHash<int, HSCCategory *> m_categories;
    QHash<int, HSCCoordinatePair *> m_coordinate_pairs;

    HMVariable * m_variable;
    Unit * m_unit;

};


}

#endif // HSC_H
