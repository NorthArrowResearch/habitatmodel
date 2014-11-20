#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <QHash>
#include <QDomDocument>
#include "namedobject.h"
#include "rastermeta.h"
#include "projectinput.h"

namespace HabitatModel{

//class HSI;
class SimulationHSCInput;

class Simulation : public NamedObjectWithID
{
public:

    /**
     * @brief Simulation
     * @param elSimulation
     */
    Simulation(QDomElement *elSimulation);

    /**
     * @brief run
     */
    virtual void Run() = 0;

    /**
     * @brief clean
     */
    virtual void Clean() = 0;

    /**
     * @brief Init
     */
    void Init();

    /**
     * @brief GetRasterExtentMeta
     * @return
     */
    inline RasterManager::RasterMeta * GetRasterExtentMeta(){ return m_RasterTemplateMeta; }

    /**
     * @brief PrepareProjectInputs create new project inputs from the global ones and put them in the stash
     */
    void PrepareProjectInputs();

    bool InputBelongs(ProjectInput);

protected:

    QString m_OutputRasterFileName;

private:

    QDir m_sfolder;

    QString m_screated_by;

    QString m_screated_on;
    QString m_shsi_source_path;

    bool m_bqueued;
    bool m_badd_individual_output;

    QHash<int, ProjectInput *> m_processed_inputs_store;

    RasterManager::RasterMeta * m_RasterTemplateMeta;
};

}


#endif // SIMULATION_H
