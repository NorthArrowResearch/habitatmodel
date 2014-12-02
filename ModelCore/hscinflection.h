#ifndef HSCINFLECTION_H
#define HSCINFLECTION_H

#include "hsc.h"
#include "hsccoordinatepair.h"
#include <QHash>

namespace HabitatModel {

class HSCInflection : public HSC
{
public:
    HSCInflection(QDomElement * elHSC);
    ~HSCInflection();

    /**
     * @brief AddCoordinatePair
     * @param nHSCID
     * @param pHSCCoordPair
     */
    inline void AddCoordinatePair(int nHSCID, HSCCoordinatePair * pHSCCoordPair){ m_coordinate_pairs.insert(nHSCID, pHSCCoordPair);  }

    /**
     * @brief GetHSValue: Gets the habitat utilization value for the specified input variable value
     * @param fInputValue
     * @return
     */
    double GetHSValue(double fInputValue);

    /**
     * @brief ProcessRaster
     * @param sInput
     * @param sOutput
     * @param sOutputRasterMeta
     */
    void ProcessRaster(QString sInput, QString sOutput, RasterManager::RasterMeta *sOutputRasterMeta);
private:

    // We have some stores of objects related to this
    // HSC that we need to look up
    QHash<int, HSCCoordinatePair *> m_coordinate_pairs;
};

} // HabitatModel

#endif // HSCINFLECTION_H
