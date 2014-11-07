#ifndef HSICURVE_H
#define HSICURVE_H

#include <QString>
#include <QHash>

#include "simulationhscinput.h"
#include "hsc.h"

namespace HabitatModel{

class HSICurve
{
public:
    /**
     * @brief HSICurve
     * @param elHSICurve the Dom node containing the HSI Curve
     */
    HSICurve(QDomElement *elHSICurve);

    inline double GetWeight() { return m_fWeight;}
    inline HSC * GetHSC(){ return m_hsc; }

private:

    HSC * m_hsc;
    double m_fWeight;

};


}

#endif // HSICURVE_H
