#ifndef HSCCOORDINATEPAIR_H
#define HSCCOORDINATEPAIR_H

#include "hsc.h"

namespace HabitatModel{

class HSCCoordinatePair
{
public:
    /**
     * @brief HSCCoordinatePair
     * @param elCoordinatePair
     */
    HSCCoordinatePair(QDomElement *elCoordinatePair);

    /**
     * @brief HSCCoordinatePair
     * @param pInput
     */
    HSCCoordinatePair(HSCCoordinatePair *pInput);

    ~HSCCoordinatePair();

    /**
     * @brief XValue
     * @return
     */
    inline double XValue() { return m_fXVal;}

    /**
     * @brief HSValue
     * @return
     */
    inline double HSValue() { return m_fHSVal;}
    /**
     * @brief HSCCoordinatePair
     * @param pInput
     */

private:
    double m_fXVal;
    double m_fHSVal;
};

}

#endif // HSCCOORDINATEPAIR_H
