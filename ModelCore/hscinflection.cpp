#include "hscinflection.h"

#include "hsccoordinatepair.h"
#include "rastermanager_interface.h"
#include <QDomElement>
#include <QHashIterator>
#include "project.h"
#include "gdal_priv.h"

namespace HabitatModel {

HSCInflection::HSCInflection(QDomElement * elHSC)
    : HSC(elHSC)
{ }

HSCInflection::~HSCInflection()
{
    // Empty the coordinate pair store
    QHashIterator<int, HSCCoordinatePair *> m(m_coordinate_pairs);
    while (m.hasNext()) {
        m.next();
        delete m.value();
    }
}

void HSCInflection::ProcessRaster(QString sInput, QString sOutput, RasterManager::RasterMeta * sOutputRasterMeta){

    Project::GetOutputXML()->Log("Processing Raster against HSC Inflection Curve: " + sInput , 2);

    Project::EnsureFile(sOutput);

    // Rasterman doesn't use Qt so we need to step down to char *
    const QByteArray sInputQB = sInput.toLocal8Bit();
    const QByteArray sOutputQB = sOutput.toLocal8Bit();
    int sRasterCols = sOutputRasterMeta->GetCols();

    GDALDataset * pInputDS = (GDALDataset*) GDALOpen( sInputQB.data(), GA_ReadOnly);
    GDALDataset * pOutputDS = RasterManager::CreateOutputDS( sOutputQB.data(), sOutputRasterMeta);

    // Allocate memory for reading from DEM
    unsigned char *pReadBuffer = (unsigned char*) CPLMalloc(sizeof(double)*sRasterCols);

    // Loop through each DEM cell
    for (int i=1; i < sOutputRasterMeta->GetRows() - 1; i++)
    {
        // Read the row
        pInputDS->GetRasterBand(1)->RasterIO(GF_Read, 0,  i,
                                           sRasterCols, 1,
                                           pReadBuffer,
                                           sRasterCols, 1,
                                           GDT_Float64, 0, 0);
        // Loop through columns
        for (int j=1; j < sRasterCols - 1; j++)
        {
            pReadBuffer[j] =  GetHSValue(pReadBuffer[j]);
        }
        pOutputDS->GetRasterBand(1)->RasterIO(GF_Write,0,i,
                                              sRasterCols,1,
                                              pReadBuffer,
                                              sRasterCols,1,
                                              GDT_Float64,
                                              0,0);
    }

    //close datasets
    GDALClose(pInputDS);
    GDALClose(pOutputDS);

    CPLFree(pReadBuffer);
    pReadBuffer = NULL;

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
