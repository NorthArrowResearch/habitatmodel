#include "hscinflection.h"
#include "project.h"
#include "hsccoordinatepair.h"
#include "rastermanager_interface.h"
#include <QDomElement>
#include <QHashIterator>

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
    m_coordinate_pairs.clear();
}

void HSCInflection::AddCoordinatePair(int nHSCID, HSCCoordinatePair *pHSCCoordPair){
    m_coordinate_pairs.insert(nHSCID, new HSCCoordinatePair(pHSCCoordPair));
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

    GDALRasterBand * pInputRB = pInputDS->GetRasterBand(1);
    GDALRasterBand * pOutputRB = pOutputDS->GetRasterBand(1);

    // Allocate memory for reading from DEM
    double * pReadBuffer = (double*) CPLMalloc(sizeof(double)*sRasterCols);

    // Loop through each DEM cell
    for (int i=0; i < sOutputRasterMeta->GetRows(); i++)
    {
        // Read the row
        pInputRB->RasterIO(GF_Read, 0,  i,
                           sRasterCols, 1,
                           pReadBuffer,
                           sRasterCols, 1,
                           GDT_Float64,
                           0, 0);

        // Loop through columns
        for (int j=0; j < sRasterCols; j++)
        {
            pReadBuffer[j] =  GetHSValue(pReadBuffer[j], pInputRB->GetNoDataValue() );
        }
        pOutputRB->RasterIO(GF_Write, 0, i,
                            sRasterCols, 1,
                            pReadBuffer,
                            sRasterCols, 1,
                            GDT_Float64,
                            0, 0);
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
double HSCInflection::GetHSValue(double fInputValue, double dNoData)
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

        if (m.value()->GetXValue() == fInputValue)
        {
            return m.value()->GetHSValue();
        }
        else
        {
            double fDifference = m.value()->GetXValue() - fInputValue;
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
    // Return noDataVal if nothing is found.
    double HSValue = dNoData;
    if (bBefore && bAfter)
    {
        HSValue = pCoordBefore->GetHSValue() + ((pCoordAfter->GetHSValue() - pCoordBefore->GetHSValue()) * ((fInputValue - pCoordBefore->GetXValue()) / (pCoordAfter->GetXValue() - pCoordBefore->GetXValue())));
    }

    // TODO: get the habitat suitability value
    return HSValue;
}

} // HabitatModel
