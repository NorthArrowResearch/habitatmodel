#ifndef HSC_H
#define HSC_H

#include <QString>
#include <QHash>

#include "namedobject.h"
#include "unit.h"
#include "hmvariable.h"
#include "hsccategory.h"
#include "hsccoordinatepair.h"

namespace HabitatModel{

class HSC : NamedObjectWithID
{
public:
    HSC();

private:
    NamedObjectWithID * m_source;
    NamedObjectWithID * m_lifestage;
    NamedObjectWithID * m_species;

    int figure;
    QString location;

    QHash<int, HSCCategory *> m_categories;
    QHash<int, HSCCoordinatePair *> m_coordinate_pairs;

    HMVariable m_variable;
    Unit m_unit;

};


}

#endif // HSC_H
