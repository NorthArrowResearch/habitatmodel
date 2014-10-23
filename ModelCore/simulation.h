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

    QString m_sfolder;
    QString m_screated_by;

    QString m_screated_on;
    QString m_shsi_source_path;

    bool m_bqueued;
    bool m_badd_individual_output;

};

}


#endif // SIMULATION_H
