#include "hsccategorical.h"
#include "hsccategory.h"
#include "project.h"
#include <QDomElement>
#include <QHashIterator>
#include "rastermanager_interface.h"
#include "rastermanager_exception.h"

namespace HabitatModel {

HSCCategorical::HSCCategorical(QDomElement * elHSC)
    : HSC(elHSC)
{ }

HSCCategorical::~HSCCategorical()
{
    // Empty the category store
    QHashIterator<int, HSCCategory *> m(m_categories);
    while (m.hasNext()) {
        m.next();
        delete m.value();
    }
    m_categories.clear();
}

double HSCCategorical::ProcessValue(double sInput, double dNoDataValue){
    return GetHSValue(sInput, dNoDataValue);
}

void HSCCategorical::ProcessRaster(QString sInput, QString sOutput, RasterManager::RasterMeta * sOutputRasterMeta){

    Project::GetOutputXML()->Log("Processing Raster against Categorical HSC: " + sInput , 2);

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
    double * pReadBuffer = (double * ) CPLMalloc(sizeof(double)*sRasterCols);

    // Loop through each DEM cell
    for (int i = 0; i < sOutputRasterMeta->GetRows(); i++)
    {
        // Read the row
        // WARNING: I'm going to try reading as a byte. if this doesn't cast doubles properly
        // Then do it manually.
        pInputRB->RasterIO( GF_Read, 0,  i,
                            sRasterCols, 1,
                            pReadBuffer,
                            sRasterCols, 1,
                            GDT_Float64, 0, 0 );

        // Loop through columns
        for (int j = 0; j < sRasterCols; j++)
        {
            if (pReadBuffer[j] != pInputRB->GetNoDataValue()) {
                pReadBuffer[j] =  GetHSValue(pReadBuffer[j], pOutputRB->GetNoDataValue());
            }
            else{
                pReadBuffer[j] = pOutputRB->GetNoDataValue();
            }

        }
        pOutputRB->RasterIO(GF_Write, 0, i,
                            sRasterCols, 1,
                            pReadBuffer,
                            sRasterCols, 1,
                            GDT_Float64,
                            0, 0 );
    }

    //Close Datasets
    GDALClose(pInputDS);
    GDALClose(pOutputDS);

    CPLFree(pReadBuffer);
    pReadBuffer = NULL;

}

double HSCCategorical::GetHSValue(double dCategory, double dNoDataVal)
{
    // This is not an integer.
    if (floor(dCategory) != dCategory){
        return dNoDataVal;
    }
    int nCategory = (int) dCategory;

    QHashIterator<int, HSCCategory *> m(m_categories);
    while (m.hasNext()) {
        m.next();

        if (m.value()->GetCategoryID() == nCategory)
            return m.value()->GetHSValue();
    }

    return dNoDataVal;
}

void HSCCategorical::AddCategory(int nHSCID, HSCCategory *pHSCCategory){
    m_categories.insert(nHSCID, new HSCCategory(pHSCCategory));
}

} // HabitatModel
