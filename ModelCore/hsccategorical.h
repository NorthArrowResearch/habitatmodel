#ifndef HSCCATEGORICAL_H
#define HSCCATEGORICAL_H

#include "hsc.h"
#include "hsccategory.h"
#include <QHash>

class HSCCategory;

namespace HabitatModel {

class HSCCategorical : public HSC
{
public:
    HSCCategorical(QDomElement * elHSC);
    ~HSCCategorical();

    /**
     * @brief GetHSValue: Get the habitat suitability value for the specified category
     * @param nCategory
     * @return
     */
    double GetHSValue(double nCategory, double dNoDataVal);

    /**
     * @brief AddCategory
     * @param nHSCID
     * @param pHSCCategory
     */
    void AddCategory(int nHSCID, HSCCategory * pHSCCategory);

    /**
     * @brief ProcessRaster
     * @param sInput
     * @param sOutput
     * @param sOutputRasterMeta
     */
    void ProcessRaster(QString sInput, QString sOutput, RasterManager::RasterMeta *sOutputRasterMeta);
    /**
     * @brief ProcessValue
     * @param sInput
     * @return
     */
    double ProcessValue(double sInput, double dNoDataValue);

private:

    QHash<int, HSCCategory *> m_categories;

};

} // Habitat Model

#endif // HSCCATEGORICAL_H
