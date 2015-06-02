#ifndef SIMULATIONFISINPUT_H
#define SIMULATIONFISINPUT_H

#include "namedobject.h"
#include <QDomElement>

namespace HabitatModel{

class SimulationFISInput : public NamedObjectWithID
{
public:
    SimulationFISInput(QDomElement elFISInput);
    ~SimulationFISInput();

};

}

#endif // SIMULATIONFISINPUT_H
