#ifndef HSI_H
#define HSI_H

#include <QString>
#include <QHash>

#include "hsicurve.h"
#include "namedobject.h"

namespace HabitatModel{

class HSI : public NamedObjectWithID
{

public:
    HSI(const char *sName);
    /**
     * @brief HSI
     * @param elSimulation the Dom node containing the HSI element
     * @param elConfig the entire configuration file root dom node
     */
    HSI(QDomElement *elHSI);
    void LoadCurves();
    HSICurve *GetCurve(int nHSICurveID);
private:
    QHash<int, HSICurve *> m_curves;
    NamedObjectWithID * m_lifestage;
    NamedObjectWithID * m_species;
    NamedObjectWithID * m_method;
};

}
#endif // HSI_H
