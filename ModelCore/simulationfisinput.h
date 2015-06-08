#ifndef SIMULATIONFISINPUT_H
#define SIMULATIONFISINPUT_H

#include "project.h"
#include "namedobject.h"
#include "projectinput.h"
#include <QDomElement>

namespace HabitatModel{

class SimulationFISInput : public NamedObjectWithID
{
public:
    SimulationFISInput(QDomElement elFISInput);
    ~SimulationFISInput();

    inline ProjectInput * GetProjectInput() const { return m_project_input; }
    inline HMVariable * GetHMVariable() const { return m_variable; }

private:

    ProjectInput * m_project_input;
    HMVariable * m_variable;


};

}

#endif // SIMULATIONFISINPUT_H