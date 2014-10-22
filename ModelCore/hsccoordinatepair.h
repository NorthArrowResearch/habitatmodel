#ifndef HSCCOORDINATEPAIR_H
#define HSCCOORDINATEPAIR_H

#include "hsc.h"

namespace HabitatModel{

class HSCCoordinatePair
{
public:
    HSCCoordinatePair(QDomElement *elCoordinatePair);
private:
    HSC * m_HSC;
    int m_nXVal;
    int m_nHSVal;
};

}

#endif // HSCCOORDINATEPAIR_H
