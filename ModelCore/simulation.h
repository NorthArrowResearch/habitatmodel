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

private:
    void LoadXML(QString qsXMLFile);

    void CreateModelFromXML();

    QDomElement theXMLOutput;
    HSI * m_hsiRef;

    QString s_folder;
    QString qd_created_by;

    QHash<int, SimulationHSCInput *> m_simulation_hsc_inputs;

    QString qd_created_on;

    bool b_queued;

    QString s_hsi_source_path;

    bool b_add_individual_output;

};

}



#endif // SIMULATION_H
