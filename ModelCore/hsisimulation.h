#ifndef HSISIMULATION_H
#define HSISIMULATION_H

#include "simulation.h"
#include "hsi.h"
#include "rastermeta.h"

namespace HabitatModel{

class HSISimulation : public Simulation
{
public:

    /**
     * @brief HSISimulation
     * @param elSimulation the dom element containing the
     * simulation
     */
    HSISimulation(QDomElement *elSimulation);

    /**
     * @brief LoadHSCInputs
     */
    void LoadHSCInputs();

    void RunSimulation();

    RasterManager::ExtentRectangle *GetRasterExtents();
private:

    void PrepareVectorRasters();
    void PrepareCSVRasters();
    void PrepareRasters();

    RasterManager::RasterMeta * m_RasterTemplateMeta;

    HSI * m_hsiRef;

    // Hash store of simulationhsc inputs related to this sim.
    QHash<int, SimulationHSCInput *> m_simulation_hsc_inputs;

};


}

#endif // HSISIMULATION_H

