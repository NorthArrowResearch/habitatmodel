#ifndef HSISIMULATION_H
#define HSISIMULATION_H

#include "simulation.h"
#include "hsi.h"
#include "rastermeta.h"

namespace HabitatModel{

enum HSIMethods {
    HSI_PRODUCT = 0,
    HSI_ARITHMETIC_MEAN = 1,
    HSI_GEOMETRIC_MEAN = 2,
    HSI_MINIMUM = 3,
    HSI_WEIGHTED_MEAN = 4,
};

class HSISimulation : public Simulation
{
public:

    /**
     * @brief HSISimulation
     * @param elSimulation the dom element containing the
     * simulation
     */
    HSISimulation(QDomElement *elSimulation);

    inline ~HSISimulation(){  }

    void LoadInputs();
    void Run();
    void Clean();
    void PrepareInputs();

    /**
     * @brief DetermineMethod
     * @param sMEthod
     * @return
     */
    int DetermineMethod();

    /**
     * @brief GetHSISourcePath
     * @return
     */
    inline QString GetHSISourcePath(){ return m_HSISourcePath; }

    /**
     * @brief AddRastersToExtents
     */
    void AddRastersToExtents();

private:

    void PrepareVectorRasters();
    void PrepareCSVRasters();
    void PrepareRasters();

    HSI * m_hsiRef;
    QString m_HSISourcePath;

    // Hash store of simulationhsc inputs related to this sim.
    QHash<int, SimulationHSCInput *> m_simulation_hsc_inputs;

};


}

#endif // HSISIMULATION_H

