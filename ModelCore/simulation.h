#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <QHash>
#include <QDomDocument>
#include "namedobject.h"

namespace HabitatModel{

class HSI;
class SimulationHSCInput;

class Simulation : public NamedObjectWithID
{
public:
    Simulation(const char * sName, int nID);

    void run();

    void clean();

    Simulation(QDomElement *elSimulation);
private:

    HSI * m_hsiRef;

    QString m_sfolder;
    QString m_screated_by;

    QHash<int, SimulationHSCInput *> m_simulation_hsc_inputs;

    QString m_screated_on;

    bool m_bqueued;

    QString m_shsi_source_path;

    bool m_badd_individual_output;

};

}


#endif // SIMULATION_H
