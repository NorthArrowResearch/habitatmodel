#ifndef SIMULATIONHSCINPUT_H
#define SIMULATIONHSCINPUT_H

namespace HabitatModel{

class HSICurve;
class ProjectInput;

class SimulationHSCInput
{
public:
    SimulationHSCInput();

private:
    ProjectInput * m_project_inputs;
    HSICurve * m_hsi_curve;
};


}
#endif // SIMULATIONHSCINPUT_H
