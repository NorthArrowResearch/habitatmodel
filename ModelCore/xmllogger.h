#ifndef XMLLOGGER_H
#define XMLLOGGER_H
#include <QFile>
#include <QDomElement>

namespace HabitatModel{

class XmlLogger
{
public:
    XmlLogger(QString sXmlFile);
    void Init();
    void Log(QString sMsg, QString sException, int nSeverity, int indent);
    void Meta(QString sTagName, QString sTagValue);

    QDomDocument *ReadLog();
    QDomDocument *ReadLogFile();
    void WriteLogFile(QDomDocument *pElement);

private:
    QFile m_xmlFile;
};
}
#endif // XMLLOGGER_H
