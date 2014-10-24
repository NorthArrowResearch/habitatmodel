#include "hscinflection.h"

#include "hsccoordinatepair.h"
#include <QDomElement>
#include <QHashIterator>
#include "project.h"

namespace HabitatModel {

HSCInflection::HSCInflection(QDomElement * elHSC)
    : HSC(elHSC)
{
    LoadCoordinatePairs();
}

HSCInflection::~HSCInflection()
{
    // Empty the coordinate pair store
    QHashIterator<int, HSCCoordinatePair *> m(m_coordinate_pairs);
    while (m.hasNext()) {
        m.next();
        delete m.value();
    }
}

void HSCInflection::LoadCoordinatePairs(){

    QDomNodeList elCoordPairs = Project::GetConfig()->elementsByTagName("HSCCoordinatePairs");

    for(int n= 0; n < elCoordPairs.length(); n++){
        QDomElement elCoordinatePair = elCoordPairs.at(n).toElement();
        int nCoordinatePairID = elCoordinatePair.firstChildElement("CoordinatePairID").text().toInt();

        HSCCoordinatePair * pCoordPair = new HSCCoordinatePair(&elCoordinatePair);
        m_coordinate_pairs.insert(nCoordinatePairID, pCoordPair);
    }
}

/* Loop over all the coordinate pairs and find the one that is closet
 * but closest before/below the desired value. Also find the one that is
 * closest after/above the desired value. Then use these before and after
 * values to interpolate the desired habitat suitability value.
 */
double HSCInflection::GetHSValue(double fInputValue)
{
    HSCCoordinatePair * pCoordBefore = NULL;
    double fDistanceBefore;
    bool bBefore = false;

    HSCCoordinatePair * pCoordAfter = NULL;
    double fDistanceAfter;
    bool bAfter = false;

    QHashIterator<int, HSCCoordinatePair *> m(m_coordinate_pairs);
    while (m.hasNext()) {
        m.next();

        if (m.value()->XValue() == fInputValue)
        {
            return m.value()->HSValue();
        }
        else
        {
            double fDifference = m.value()->XValue() - fInputValue;
            if (fDifference > 0)
            {
                // The inflection point is greater than the desired value
                if (!bAfter || fDifference < fDistanceAfter)
                {
                    fDistanceAfter = fDifference;
                    pCoordAfter = m.value();
                    bAfter = true;
                }
            }
            else
            {
                // The inflection point is less than the desired value
                if (!bBefore || fDifference > fDistanceBefore)
                {
                    fDistanceBefore = fDifference;
                    pCoordBefore = m.value();
                    bBefore = true;
                }
            }
        }
    }

    // Check if we have a before and after value, then interpolate
    double HSValue = 0;
    if (bBefore && bAfter)
    {
        HSValue = pCoordBefore->HSValue() + ((pCoordAfter->HSValue() - pCoordBefore->HSValue()) * ((fInputValue - pCoordBefore->XValue()) / (pCoordAfter->XValue() - pCoordBefore->XValue())));
    }

    // TODO: get the habitat suitability value
    return HSValue;
}

} // HabitatModel
