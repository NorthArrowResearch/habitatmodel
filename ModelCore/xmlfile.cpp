#include "xmlfile.h"
#include "exception"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDomElement>
#include <QXmlStreamWriter>
#include "habitat_exception.h"

namespace HabitatModel{

XMLFile::XMLFile(QString sXmlFile, bool bInput)
{
    // Is it an input file or an output file?
    if (bInput && QFile(sXmlFile).exists())
        Load(sXmlFile);
    else if (!bInput && !QFile(sXmlFile).exists())
        Init(sXmlFile);

    //Something is wrong. Throw an exception
    else if (bInput && !QFile(sXmlFile).exists())
        throw HabitatException(FILE_NOT_FOUND, sXmlFile);
    else
        throw HabitatException(FILE_PRESENT, sXmlFile);
}

XMLFile::~XMLFile(){

    if (!m_xmlFile == NULL)
    {
        if (m_xmlFile->isOpen())
            m_xmlFile->close();

        delete m_xmlFile;
    }
    delete m_pDoc;
}

void XMLFile::Load(QString &sFilePath)
{
    if (sFilePath.isEmpty() || sFilePath.isNull())
        throw "The file path is null or empty";
    else
        if (!QFile::exists(sFilePath))
            throw "The GCD project file does not exist.";

    m_pDoc = new QDomDocument;
    m_xmlFile = new QFile(sFilePath);
    if (!m_xmlFile->open(QIODevice::ReadOnly))
        throw "Failed to open the GCD XML project file as read only.";

    if (!m_pDoc->setContent(m_xmlFile->readAll())) {
        m_xmlFile->close();
        throw "Failed to load DOM from GCD XML project file.";
    }
}


// Create the file with the base skeleton we need. Then close it.
void XMLFile::Init(QString &sFilePath){

    m_pDoc = new QDomDocument;
    m_xmlFile = new QFile(sFilePath);

    /*open a file */
    if (!m_xmlFile->open(QIODevice::WriteOnly))
        throw HabitatException(FILE_READ_ONLY, m_xmlFile->fileName());
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

void XMLFile::AddMeta(QString sTagName, QString sTagValue){
    QDomElement meta_data, meta_data_tag;
    QDomElement documentElement = m_pDoc->documentElement();
    QDomNodeList elements = documentElement.elementsByTagName( "meta_data" );

    if( elements.size() == 0 )
    {
      meta_data = m_pDoc->createElement( "meta_data" );
      documentElement.insertBefore( meta_data, documentElement );
    }
    else if( elements.size() == 1 )
    {
      meta_data = elements.at(0).toElement();
    }

    // Create the message itself
    meta_data_tag = m_pDoc->createElement( sTagName );
    meta_data_tag.setNodeValue(sTagValue);
    meta_data.appendChild( meta_data_tag );

    WriteDomToFile();
}

void XMLFile::Log(QString sMsg, QString sException, int nSeverity, int indent)
{
    QDomElement messages, message, exception, description;
    QDomElement documentElement = m_pDoc->documentElement();
    QDomNodeList elements = documentElement.elementsByTagName( "messages" );

    if( elements.size() == 0 )
    {
      messages = m_pDoc->createElement( "messages" );
      documentElement.insertBefore( messages, documentElement );
    }
    else if( elements.size() == 1 )
    {
      messages = elements.at(0).toElement();
    }

    // Create the message itself
    message = m_pDoc->createElement( "message" );
    message.setAttribute( "severity", nSeverity );
    message.setAttribute( "indent", indent );

    // Now create the description of the message
    description = m_pDoc->createElement( "description" );
    description.setNodeValue(sMsg);
    message.appendChild( description );

    // Only create an exception if we need to.
    if (nSeverity != 0 && sException.compare("") != 0){
        exception = m_pDoc->createElement( "exception" );
        exception.setNodeValue(sException);
        message.appendChild( exception );
    }
    messages.appendChild( message );
    WriteDomToFile();
}


void XMLFile::WriteDomToFile(){

    // Great. Now Write the domelement to the file.
    if( !m_xmlFile->open( QIODevice::WriteOnly | QIODevice::Text ) )
        throw HabitatException(FILE_WRITE_ERROR,  m_xmlFile->fileName());

    QTextStream stream( m_xmlFile );
    stream << m_pDoc->toString();
    m_xmlFile->close();

}


}
