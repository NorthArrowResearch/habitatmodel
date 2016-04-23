#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <QHash>
#include <QDomDocument>
#include "histogramsclass.h"
#include "namedobject.h"
#include "projectinput.h"

namespace HabitatModel{

//class HSI;
class SimulationHSCInput;
class XMLFile;

class Simulation : public NamedObjectWithID
{
public:

    /**
     * @brief Simulation
     * @param elSimulation
     */
    Simulation(QDomElement *elSimulation);

    virtual ~Simulation();

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

    inline bool HasOutputRaster(){ return m_bOutputRaster.compare("") != 0;}
    inline bool HasOutputCSV(){ return m_bOutputCSV.compare("") != 0;  }

    inline int GetHistogramBins(){ return m_nHistogramBins;  }

    inline QString GetFolder(){ return m_sfolder; }

    /**
     * @brief GetRasterExtentMeta
     * @return
     */
    inline RasterManager::RasterMeta * GetRasterExtentMeta(){ return m_RasterTemplateMeta; }

    bool InputBelongs(ProjectInput *pInput);

protected:

    void RasterUnion(RasterManager::RasterMeta *pMeta);

    // These wrap ProjectError
    void SimulationError(int nErrorCode);
    void SimulationError(int nErrorCode, QString m_sEvidence);
    void SimulationLog(QString sMsg, int nIndent);
    void SimulationAddResult(QString sTagName, QString sTagValue);

    QString m_bOutputRaster;
    QString m_bOutputHistogram;
    QString m_bOutputCSV;

    XMLFile * m_XMLSimOutput;

    int m_NumRasters;
    int m_NumCSVs;
    int m_NumVectors;

    int m_nHistogramBins;

    // Results Variables
    double m_dWeightedUse;
    double m_dNormWeightedUse;
    double m_dPercentUsage;
    int m_nOccupiedCells;
    int m_nTotalCells;
    int m_nCSVLines;
    double m_dCellArea;

    // On CSV runs we need to know cell size
    double m_dCellSize;

    void SimulationAddHistogram(RasterManager::HistogramsClass theHisto);
private:
    /**
     * @brief Init
     */
    void Init();

    void InitRasterMeta(QDomElement *elSimulation);

    /**
     * @brief m_sfolder the simulation folder relative to project root
     */
    QString m_sfolder;
    QString m_screated_by;

    QString m_screated_on;

    bool m_badd_individual_output;

    RasterManager::RasterMeta * m_RasterTemplateMeta;
};

}


#endif // SIMULATION_H
