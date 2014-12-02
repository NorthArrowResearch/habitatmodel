#include "hsccategorical.h"
#include "hsccategory.h"
#include "project.h"
#include <QDomElement>
#include <QHashIterator>
#include "rastermanager_interface.h"

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

    // Allocate memory for reading from DEM
    unsigned char *pReadBuffer = (unsigned char*) CPLMalloc(sizeof(int)*sRasterCols);

    // Loop through each DEM cell
    for (int i=1; i < sOutputRasterMeta->GetRows() - 1; i++)
    {
        // Read the row
        // WARNING: I'm going to try reading as a byte. if this doesn't cast doubles properly
        // Then do it manually.
        pInputDS->GetRasterBand(1)->RasterIO(GF_Read, 0,  i,
                                           sRasterCols, 1,
                                           pReadBuffer,
                                           sRasterCols, 1,
                                           GDT_Byte, 0, 0);

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

double HSCCategorical::GetHSValue(int nCategory)
{
    QHashIterator<int, HSCCategory *> m(m_categories);
    while (m.hasNext()) {
        m.next();

        if (m.value()->GetCategory() == nCategory)
            return m.value()->GetHSValue();
    }

    return 0;
}

} // HabitatModel
