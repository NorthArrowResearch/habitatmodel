#ifndef XMLFILE_H
#define XMLFILE_H
#ifdef LINUX
#include <unistd.h>
#endif
#ifdef WINDOWS
#include <windows.h>
#endif


#include <QFile>
#include <QDomElement>
#include "simulation.h"
#include "histogramsclass.h"



namespace HabitatModel{

enum XML_LOG_SEVERITY {
    SEVERITY_SUCCESS = 0,
    SEVERITY_WARNING = 1,
    SEVERITY_ERROR = 2,
    SEVERITY_VERBOSE = 3,
};

enum StatusCode {
    STATUS_INITIALIZED
    , STATUES_LOADED
    , STATUS_PREPARED
    , STATUS_COMPLETE
};

enum StatusType {
    STATUSTYPE_PROJECT
    , STATUSTYPE_SIMULATION
};


inline QString enumToString(StatusCode eCode){
    switch (eCode){
    case STATUS_INITIALIZED: return "Initialized";
    case STATUES_LOADED: return "Loaded";
    case STATUS_PREPARED: return "Prepared Inputs";
    case STATUS_COMPLETE: return "Complete";
    default: return "Unknown Status Code";
    }
}

inline QString enumToString(StatusType eCode){
    switch (eCode){
    case STATUSTYPE_PROJECT: return "Project";
    case STATUSTYPE_SIMULATION: return "Simulation";
    default: return "unknown";
    }
}


inline QString enumToString(XML_LOG_SEVERITY eCode){
    switch (eCode){
    case SEVERITY_SUCCESS: return "Success";
    case SEVERITY_WARNING: return "Warning";
    case SEVERITY_ERROR: return "Error";
    case SEVERITY_VERBOSE: return "Verbose";
    default: return "unknown";
    }
}

class XMLFile
{

public:
    XMLFile(QString sXmlFile, bool bInput);
    ~XMLFile();

    /**
     * @brief Init
     * @param sFilePath
     */
    void Init(QString &sFilePath);
    /**
     * @brief Load
     * @param sFilePath
     */
    void Load(QString &sFilePath);
    /**
     * @brief Log : Write the <message> log files
     * @param sMsg
     * @param sException
     * @param nSeverity
     * @param indent
     */
    void Log(QString sMsg, QString sException, int nSeverity, int indent);
    /**
     * @brief Log : Convenience overload for when we have a message only
     * @param sMsg
     */
    inline void Log(QString sMsg){ Log(sMsg, "", SEVERITY_SUCCESS, 0); }

    /**
     * @brief Log : Convenience overload for when we have a message and an indent
     * @param sMsg
     * @param indent
     */
    inline void Log(QString sMsg, int indent){ Log(sMsg, "", SEVERITY_SUCCESS, indent); }

    /**
     * @brief LogDebug
     * @param sMsg
     * @param indent
     */
    inline void LogDebug(QString sMsg, int indent){ Log(sMsg, "", SEVERITY_VERBOSE, indent); }

    /**
     * @brief Meta: Write the meta_data parameters one at a time.
     * @param sTagName
     * @param sTagValue
     */
    void AddRunMeta(QString sTagName, QString sTagValue);

    /**
     * @brief AddResult Add a <result> tag
     * @param sTagName
     * @param sTagValue
     */
    void AddResult(Simulation * logSim, QString sTagName, QString sTagValue);

    /**
     * @brief WriteDomToFile
     */
    void WriteDomToFile();

    /**
     * @brief Document
     * @return
     */
    inline QDomDocument * Document(){ return m_pDoc; }

    /**
     * @brief GetTmpFileName
     * @return
     */
    QString GetTmpFileName();

    /**
     * @brief CopyTmpToOutput update the real file with a copy of the tmp file
     */
    void CopyTmpToOutput();

    /**
     * @brief AddStatus
     * @param sID
     * @param nCode
     * @param nType
     * @param time
     */
    void AddStatus(QString sID, StatusCode nCode, StatusType nType, int nTime);
    /**
     * @brief QueueStatus
     * @param sID
     * @param nCode
     * @param nType
     * @param nTime
     */
    void QueueStatus(QString sID, StatusCode nCode, StatusType nType, int nTime);

    /**
     * @brief WriteHistogram
     * @param theHisto
     * @param logSim
     */
    void WriteHistogram(RasterManager::HistogramsClass theHisto, Simulation *logSim);

    /**
     * @brief WriteSimulationMeta
     * @param logsim
     */
    void WriteSimulationMeta(Simulation *logSim);

private:
    QFile * m_xmlFile;

    QString m_sXMLFilePath;
    QString m_sTMPFilePath;

    QDomDocument * m_pDoc;

    inline void mySleep(int sleepMs)
    {
    sleepMs += 0;
    #ifdef LINUX
        usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)
    #endif
    #ifdef WINDOWS
        Sleep(sleepMs);
    #endif
    }

    /**
     * @brief GetSimulationNode
     * @param logSim
     * @return
     */
    QDomElement GetSimulationNode(Simulation *logSim);
};


}
#endif // XMLLOGGER_H
