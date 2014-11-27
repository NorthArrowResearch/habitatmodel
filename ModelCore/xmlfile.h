#ifndef XMLFILE_H
#define XMLFILE_H
#include <QFile>
#include <QDomElement>

namespace HabitatModel{

enum XML_LOG_SEVERITY {
    SEVERITY_SUCCESS = 0,
    SEVERITY_WARNING = 1,
    SEVERITY_ERROR = 2
};

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
     * @brief Meta: Write the meta_data parameters one at a time.
     * @param sTagName
     * @param sTagValue
     */
    void AddMeta(QString sTagName, QString sTagValue);

    /**
     * @brief WriteDomToFile
     */
    void WriteDomToFile();

    /**
     * @brief Document
     * @return
     */
    inline QDomDocument * Document(){ return m_pDoc; }

private:
    QFile * m_xmlFile;
    QDomDocument * m_pDoc;
};

}
#endif // XMLLOGGER_H
