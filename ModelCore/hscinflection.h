#ifndef HSCINFLECTION_H
#define HSCINFLECTION_H

#include "hsc.h"
#include "hsccoordinatepair.h"
#include <QHash>

namespace HabitatModel {

class HSCInflection : HSC
{
public:
    HSCInflection(QDomElement * elHSC);
    ~HSCInflection();

    // Gets the habitat utilization value for the specified input variable value
    double GetHSValue(double fInputValue);

private:
    /**
     * @brief LoadCoordinatePairs load all related
     * coordinate pairs using the XML config
     * file
     */
    void LoadCoordinatePairs();


    // We have some stores of objects related to this
    // HSC that we need to look up
    QHash<int, HSCCoordinatePair *> m_coordinate_pairs;
};

} // HabitatModel

#endif // HSCINFLECTION_H
