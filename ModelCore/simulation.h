#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <QHash>
#include <QDomDocument>
#include "namedobject.h"
#include "projectinput.h"
// We need a complete type in order to destruct RasterMeta.
// Find a way to make this work in windows without removing this include
#include "rastermeta.h"

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

    virtual inline ~Simulation(){
        delete m_RasterTemplateMeta;
    }

    /**
     * @brief run
     */
    virtual void Run() = 0;

    /**
     * @brief clean
     */
    virtual void Clean() = 0;

    virtual void LoadInputs() = 0;

    virtual void PrepareInputs() = 0;

    /**
     * @brief Init
     */
    void Init();

    inline bool HasRasters(){ return m_HasRasters; }

    /**
     * @brief GetRasterExtentMeta
     * @return
     */
    inline RasterManager::RasterMeta * GetRasterExtentMeta(){ return m_RasterTemplateMeta; }

    bool InputBelongs(ProjectInput *pInput);


protected:

    void RasterUnion(RasterManager::RasterMeta *pMeta);

    QString m_OutputRasterFileName;
    bool m_HasRasters;

private:
    /**
     * @brief m_sfolder the simulation folder relative to project root
     */
    QString m_sfolder;
    QString m_screated_by;

    QString m_screated_on;

    bool m_bqueued;
    bool m_badd_individual_output;

    RasterManager::RasterMeta * m_RasterTemplateMeta;
};

}


#endif // SIMULATION_H
