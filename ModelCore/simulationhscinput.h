#ifndef SIMULATIONHSCINPUT_H
#define SIMULATIONHSCINPUT_H

namespace HabitatModel{

class HSICurve;
class ProjectInput;

class SimulationHSCInput
{
public:
    /**
     * @brief SimulationHSCInput
     * @param p_HSICurve
     * @param p_ProjectInput
     */
    SimulationHSCInput(HSICurve * p_HSICurve, ProjectInput * p_ProjectInput);

    /**
     * @brief GetHSICurve
     * @return
     */
    HSICurve * GetHSICurve();

    /**
     * @brief GetProjectInput
     * @return
     */
    ProjectInput * GetProjectInput();
private:

    ProjectInput * m_project_input;
    HSICurve * m_hsi_curve;

};


}
#endif // SIMULATIONHSCINPUT_H
