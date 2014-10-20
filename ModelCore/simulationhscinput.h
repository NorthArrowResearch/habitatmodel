#ifndef SIMULATIONHSCINPUT_H
#define SIMULATIONHSCINPUT_H

#include "hsicurve.h"
#include "projectinput.h"
#include "simulation.h"

namespace HabitatModel{

class SimulationHSCInput
{
public:
    SimulationHSCInput();

private:
    ProjectInput m_project_inputs;
    HSICurve m_hsi_curve;
};


}
#endif // SIMULATIONHSCINPUT_H
