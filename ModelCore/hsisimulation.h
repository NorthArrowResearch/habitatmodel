#ifndef HSISIMULATION_H
#define HSISIMULATION_H

#include "simulation.h"

namespace HabitatModel{

class HSISimulation : public Simulation
{
public:

    HSISimulation(QDomElement *elSimulation);

private:
    HSI * m_hsiRef;

};

#endif // HSISIMULATION_H

}
