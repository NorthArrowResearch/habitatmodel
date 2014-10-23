#include "simulationhscinput.h"
#include "modelengine.h"
#include "hsicurve.h"
#include "projectinput.h"

namespace HabitatModel{

SimulationHSCInput::SimulationHSCInput(HSICurve * p_HSICurve, ProjectInput * p_ProjectInput)
{
    m_hsi_curve = p_HSICurve;
    m_project_input = p_ProjectInput;
}

HSICurve * SimulationHSCInput::GetHSICurve(){
    return m_hsi_curve;
}

ProjectInput * SimulationHSCInput::GetProjectInput(){
    return m_project_input;
}

}

