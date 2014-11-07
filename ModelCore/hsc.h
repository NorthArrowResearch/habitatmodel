#ifndef HSC_H
#define HSC_H

#include <QString>
#include "namedobject.h"

namespace HabitatModel{

class Unit;
class HMVariable;

class HSC : public NamedObjectWithID
{
public:

    /**
     * @brief HSC
     * @param elHSC dom element containing an HSC
     */
    HSC(QDomElement *elHSC);


private:
    NamedObjectWithID * m_source;
    NamedObjectWithID * m_lifestage;
    NamedObjectWithID * m_species;

    int m_figure;
    QString m_location;

    HMVariable * m_variable;
    Unit * m_unit;

};


}

#endif // HSC_H
