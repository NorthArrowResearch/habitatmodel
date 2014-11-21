#ifndef XMLLOGGER_H
#define XMLLOGGER_H
#include <QFile>
#include <QDomElement>

namespace HabitatModel{

enum XML_LOG_SEVERITY {
    SEVERITY_SUCCESS = 0,
    SEVERITY_WARNING = 1,
    SEVERITY_ERROR = 2
};

class XmlLogger
{
public:
    XmlLogger(QString sXmlFile);
    ~XmlLogger();
    void Init();

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
    void Meta(QString sTagName, QString sTagValue);

    /**
     * @brief ReadLog
     * @return
     */
    void ReadLogFile(QDomDocument *document);

    /**
     * @brief WriteLogFile
     * @param pElement
     */
    void WriteLogFile(QDomDocument *pElement);

    /**
     * @brief qXMLDebug
     * @param sMsg
     */
    void qXMLDebug(QString sMsg);

private:
    QFile * m_xmlFile;
};

}
#endif // XMLLOGGER_H
