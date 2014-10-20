#ifndef HSI_H
#define HSI_H

#include <QString>
#include <QHash>

#include "hsicurve.h"
#include "namedobject.h"

namespace HabitatModel{

class HSI : public NamedObject
{

public:
    HSI(const char *sName);
private:
    QHash<int, HSICurve *> m_curves;
    NamedObjectWithID * m_lifestage;
    NamedObjectWithID * m_species;
    NamedObjectWithID * m_method;
};

}
#endif // HSI_H
