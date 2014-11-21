#include "xmllogger.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDomElement>
#include <QXmlStreamWriter>

namespace HabitatModel{

XmlLogger::XmlLogger(QString sXmlFile)
{
    m_xmlFile = new QFile(sXmlFile);
    Init();
}

XmlLogger::~XmlLogger(){
    delete m_xmlFile;
}

// Create the file with the base skeleton we need. Then close it.
void XmlLogger::Init(){

    /*open a file */
    if (!m_xmlFile->open(QIODevice::WriteOnly))
    {
        /* show wrror message if not able to open file */
        qXMLDebug("The file is in read only mode");
    }
    else
    {
        /*if file is successfully opened then create XML*/
        QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
        /* set device (here file)to streamwriter */
        xmlWriter->setDevice(m_xmlFile);
        /* Writes a document start with the XML version number version. */
        xmlWriter->writeStartDocument();
        /* Writes a start element with name,
        * Subsequent calls to writeAttribute() will add attributes to this element. */
        xmlWriter->writeStartElement("log");

        xmlWriter->writeStartElement("meta_data");
        xmlWriter->writeEndElement(); // </meta_data>

        xmlWriter->writeStartElement("mesages");
        xmlWriter->writeEndElement(); // </messages>

        xmlWriter->writeEndElement(); // </log>

        /*end document */
        xmlWriter->writeEndDocument();
        delete xmlWriter;
    }
}

void XmlLogger::Meta(QString sTagName, QString sTagValue){
    QDomDocument * document;
    ReadLogFile(document);
    QDomElement meta_data, meta_data_tag;
    QDomElement documentElement = document->documentElement();
    QDomNodeList elements = documentElement.elementsByTagName( "meta_data" );

    if( elements.size() == 0 )
    {
      meta_data = document->createElement( "meta_data" );
      documentElement.insertBefore( meta_data, documentElement );
    }
    else if( elements.size() == 1 )
    {
      meta_data = elements.at(0).toElement();
    }

    // Create the message itself
    meta_data_tag = document->createElement( sTagName );
    meta_data_tag.setNodeValue(sTagValue);
    meta_data.appendChild( meta_data_tag );

    WriteLogFile(document);
}

void XmlLogger::Log(QString sMsg, QString sException, int nSeverity, int indent)
{
    QDomDocument * document;
    ReadLogFile(document);
    QDomElement messages, message, exception, description;
    QDomElement documentElement = document->documentElement();
    QDomNodeList elements = documentElement.elementsByTagName( "messages" );

    if( elements.size() == 0 )
    {
      messages = document->createElement( "messages" );
      documentElement.insertBefore( messages, documentElement );
    }
    else if( elements.size() == 1 )
    {
      messages = elements.at(0).toElement();
    }

    // Create the message itself
    message = document->createElement( "message" );
    message.setAttribute( "severity", nSeverity );
    message.setAttribute( "indent", indent );

    // Now create the description of the message
    description = document->createElement( "description" );
    description.setNodeValue(sMsg);
    message.appendChild( description );

    // Only create an exception if we need to.
    if (nSeverity != 0 && sException.compare("") != 0){
        exception = document->createElement( "exception" );
        exception.setNodeValue(sException);
        message.appendChild( exception );
    }
    messages.appendChild( message );
    WriteLogFile(document);
}

void XmlLogger::ReadLogFile(QDomDocument * document){

    if( !document->setContent( m_xmlFile ) )
    {
        qXMLDebug("Failed to parse the log file into a DOM tree");
        m_xmlFile->close();
    }
    m_xmlFile->close();
}

void XmlLogger::WriteLogFile(QDomDocument * pElement){

    // Great. Now Write the domelement to the file.
    if( !m_xmlFile->open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        qDebug("%s %s", "Failed to open log file for writing:", qPrintable(m_xmlFile->fileName()) ); // << xmlFileInfo.fileName()
    }
    QTextStream stream( m_xmlFile );
    stream << pElement->toString();
    m_xmlFile->close();
}

void XmlLogger::qXMLDebug(QString sMsg){
    qDebug( "%s : %s", qPrintable(sMsg), qPrintable( m_xmlFile->fileName() ) );
}


}