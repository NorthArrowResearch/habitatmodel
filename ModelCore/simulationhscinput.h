#ifndef SIMULATIONHSCINPUT_H
#define SIMULATIONHSCINPUT_H
#include "namedobject.h"
#include "unit.h"
#include "hmvariable.h"

namespace HabitatModel{

class HSICurve;
class ProjectInput;

/**
 * @brief The SimulationHSCInput class encompasses both SimulationHSCInputs
 *  and ProjectVariables in the inputs.
 */
class SimulationHSCInput : public NamedObjectWithID
{
public:
    /**
     * @brief SimulationHSCInput
     * @param p_HSICurve
     * @param p_ProjectInput
     */
    SimulationHSCInput(QDomElement elHSCInput, HSICurve *pHSICurve);

    inline ProjectInput * GetProjectInput(){ return m_project_input; }
    inline HMVariable * GetHMVariable(){ return m_variable; }
    inline HSICurve * GetHSICurve(){ return m_hsi_curve; }
    inline Unit * GetUnit(){ return m_units; }


    ~SimulationHSCInput();
private:

    ProjectInput * m_project_input;
    HMVariable * m_variable;
    HSICurve * m_hsi_curve;
    Unit * m_units;

};


}
#endif // SIMULATIONHSCINPUT_H
