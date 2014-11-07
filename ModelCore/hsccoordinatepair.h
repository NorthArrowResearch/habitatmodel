#ifndef HSCCOORDINATEPAIR_H
#define HSCCOORDINATEPAIR_H

#include "hsc.h"

namespace HabitatModel{

class HSCCoordinatePair
{
public:
    HSCCoordinatePair(QDomElement *elCoordinatePair);

    inline double XValue() { return m_fXVal;}
    inline double HSValue() { return m_fHSVal;}

private:
    double m_fXVal;
    double m_fHSVal;
};

}

#endif // HSCCOORDINATEPAIR_H
