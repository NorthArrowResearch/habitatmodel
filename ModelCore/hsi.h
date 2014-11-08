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

    /**
     * @brief HSI
     * @param elSimulation the Dom node containing the HSI element
     * @param elConfig the entire configuration file root dom node
     */
    HSI(QDomElement *elHSI);

    ~HSI();

    /**
     * @brief LoadCurves load all related
     * HSI Curves using the original XML config file
     */
    void LoadCurves();

    /**
     * @brief GetCurve
     * @param nHSICurveID
     * @return
     */
    inline HSICurve * GetCurve(int nHSICurveID){ return m_curves.value(nHSICurveID); }

    inline NamedObjectWithID * GetLifeStage() { return m_lifestage; }
    inline NamedObjectWithID * GetSpecies() { return m_species; }
    inline NamedObjectWithID * GetMethod() { return m_method; }

private:

    NamedObjectWithID * m_lifestage;
    NamedObjectWithID * m_species;
    NamedObjectWithID * m_method;

    // Store of related HSI Curves
    QHash<int, HSICurve *> m_curves;

};

}
#endif // HSI_H
