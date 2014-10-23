#ifndef SIMULATIONHSCINPUT_H
#define SIMULATIONHSCINPUT_H

namespace HabitatModel{

class HSICurve;
class ProjectInput;

class SimulationHSCInput
{
public:
    SimulationHSCInput(HSICurve * p_HSICurve, ProjectInput * p_ProjectInput);
    HSICurve * GetHSICurve();
    ProjectInput * GetProjectInput();
private:
    ProjectInput * m_project_input;
    HSICurve * m_hsi_curve;
};


}
#endif // SIMULATIONHSCINPUT_H
