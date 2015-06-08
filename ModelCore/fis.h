#ifndef FIS_H
#define FIS_H

#include <QString>
#include "fiscore.h"
#include <QFile>
#include <vector>
#include "gdal_priv.h"
#include "namedobject.h"
#include "project.h"
#include "simulation.h"
#include "rastermanager_interface.h"
#include "habitat_exception.h"
#include "simulationfisinput.h"

namespace HabitatModel{

class FIS : public NamedObjectWithID
{
public:
    FIS(QDomElement *elFIS, QHash<int, SimulationFISInput *> *pSimulationFisInputs, RasterManager::RasterMeta *RasterExtents);
    ~FIS();

    void init();
    void init(const char *fisFilename, const char *driver, float noData, const char *dodFilename, FISRuleSet *rules);
    void RunRasterFis(QString sOutputFile);
private:

    QString m_sFISRuleFile;
    FISRuleSet* rules; /**< TODO */
    QHash<int, SimulationFISInput *> * m_pfis_inputs;

    GDALDataset * m_outputDataset;
    RasterManager::RasterMeta * m_RasterExtents;

    //GDALDataset* fisDataset; /**< TODO */

};

}

#endif // FIS_H
