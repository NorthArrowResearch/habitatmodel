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
    HSICurve();

private:
    HSC * m_hsc;
    double weight;
};


}

#endif // HSICURVE_H
