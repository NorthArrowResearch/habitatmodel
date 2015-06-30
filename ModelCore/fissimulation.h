#ifndef FISSIMULATION_H
#define FISSIMULATION_H

#include "simulationfisinput.h"
#include <QDomElement>
#include <QHash>
#include "gdal_priv.h"
#include "fiscore.h"

namespace HabitatModel{

class FISSimulation : public Simulation
{
public:
    FISSimulation(QDomElement *elSimulation);
    ~FISSimulation();

    void LoadInputs();
    void Run();
    void Clean();
    void PrepareInputs();

    inline SimulationFISInput * GetSimulationFISInput(int nIndex) const { return m_simulation_fis_inputs.value(nIndex); }

    void RunRasterFis(QString sOutputFile);
    void RunCSVFis(QString sOutputFile);
private:

     // Hash store of simulationFIS inputs related to this sim.
    QHash<int, SimulationFISInput *> m_simulation_fis_inputs;

    void AddRastersToExtents();


    QString m_sFISRuleFile;
    FISRuleSet* rules; /**< TODO */

    GDALDataset * m_outputDataset;

};

}

#endif // FISSIMULATION_H
