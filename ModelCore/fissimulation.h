#ifndef FISSIMULATION_H
#define FISSIMULATION_H

#include "simulation.h"
#include "simulationfisinput.h"
#include "projectinputraster.h"
#include "projectinputcsv.h"
#include "projectinputvector.h"
#include "fis.h"
#include <QDomElement>
#include <QHash>


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

    inline FIS * GetFIS() const { return m_fis; }
    inline SimulationFISInput * GetSimulationFISInput(int nIndex) const { return m_simulation_fis_inputs.value(nIndex); }

private:

    FIS * m_fis;

     // Hash store of simulationFIS inputs related to this sim.
    QHash<int, SimulationFISInput *> m_simulation_fis_inputs;

};

}

#endif // FISSIMULATION_H
