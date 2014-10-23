#ifndef HSISIMULATION_H
#define HSISIMULATION_H

#include "simulation.h"

namespace HabitatModel{

class HSISimulation : public Simulation
{
public:

    HSISimulation(QDomElement *elSimulation);

    void LoadHSCInputs();
private:
    HSI * m_hsiRef;
    QHash<int, SimulationHSCInput *> m_simulation_hsc_inputs;
};

#endif // HSISIMULATION_H

}
