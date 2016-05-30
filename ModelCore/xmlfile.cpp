#include "xmlfile.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDateTime>
#include <QDomElement>
#include <QXmlStreamWriter>
#include <QDebug>
#include "habitat_exception.h"
#include "histogramsclass.h"
#include "simulation.h"
#include "habitat_interface.h"
#include "rastermanager_interface.h"

namespace HabitatModel{

QString sTmpPrefix = "_TMP_OUT_";

XMLFile::XMLFile(QString sXmlFile, bool bInput)
{
    // Is it an input file or an output file?
    if (bInput && QFile(sXmlFile).exists())
        Load(sXmlFile);
    else if (!bInput)
        Init(sXmlFile);

    //Something is wrong. Throw an exception
    else if (bInput && !QFile(sXmlFile).exists())
        throw HabitatException(FILE_NOT_FOUND, sXmlFile);
    else
        throw HabitatException(FILE_PRESENT, sXmlFile);
}

XMLFile::~XMLFile(){

    if (m_xmlFile != NULL)
    {
        if (m_xmlFile->isOpen())
            m_xmlFile->close();

        delete m_xmlFile;
    }
    // Now clean up the temparary file.
    if (QFile::exists(m_sTMPFilePath))
    {
        QFile::remove(m_sTMPFilePath);
    }

    // Delete the TMP file
    delete m_pDoc;
}

void XMLFile::Load(QString &sFilePath)
{
    if (sFilePath.isEmpty() || sFilePath.isNull())
        throw HabitatException(FILE_NOT_FOUND, "Filepath was empty");
    else
        if (!QFile::exists(sFilePath))
            throw HabitatException(FILE_NOT_FOUND, sFilePath);

    m_pDoc = new QDomDocument;
    m_xmlFile = new QFile(sFilePath);
    if (!m_xmlFile->open(QIODevice::ReadOnly))
        throw HabitatException(FILE_READ_ONLY, sFilePath);

    if (!m_pDoc->setContent(m_xmlFile->readAll())) {
        m_xmlFile->close();
        throw HabitatException(DOM_LOAD_ERROR, sFilePath);
    }
}


void XMLFile::CopyTmpToOutput(){
    try{
        if (QFile::exists(m_sXMLFilePath))
        {
            QFile::remove(m_sXMLFilePath);
        }

        QFile::copy(m_sTMPFilePath, m_sXMLFilePath);
    }catch(std::exception e)
    {
        //Do nothing. We don't care that it can't write.
    }
}

// Create the file with the base skeleton we need. Then close it.
void XMLFile::Init(QString &sFilePath){

    m_pDoc = new QDomDocument;

    m_sXMLFilePath = sFilePath;

    QFileInfo sNewFileInfo(sFilePath);
    QDir sNewDir = QDir(sNewFileInfo.absolutePath());

    // Clean up all older TMP files
    QStringList slTmpFiles;

    QString fileName = sTmpPrefix + "*.xml";
    slTmpFiles = sNewDir.entryList(QStringList(fileName),
                                   QDir::Files | QDir::NoSymLinks);

    foreach(QString sTmpFile, slTmpFiles){
        // If the file was already there remove it
        QString sTmpFilePath = QDir(sNewDir).filePath( sTmpFile );
        if (QFile::exists(sTmpFilePath))
            QFile::remove(sTmpFilePath);
    }


    m_sTMPFilePath = QDir(sNewDir).filePath( GetTmpFileName() );

    m_xmlFile = new QFile(m_sTMPFilePath);

    /*open a file */
    if (!m_xmlFile->open(QIODevice::WriteOnly))
        throw HabitatException(FILE_READ_ONLY, m_xmlFile->fileName());
    else
    {

        QDomElement log = m_pDoc->createElement("log");
        QDomElement habVersion = m_pDoc->createElement("habitat_version");
        QDomElement rmVersion = m_pDoc->createElement("rasterman_version");
        QDomElement runTimestamp = m_pDoc->createElement("run_timestamp");
        QDomElement status = m_pDoc->createElement("status");
        QDomElement simulations = m_pDoc->createElement("simulations");
        QDomElement meta = m_pDoc->createElement("run_meta");
        QDomElement messages = m_pDoc->createElement("messages");

        habVersion.appendChild(m_pDoc->createTextNode(GetLibVersion()));
        rmVersion.appendChild(m_pDoc->createTextNode(RasterManager::GetLibVersion()));

        // Timestamps are useful
        QDateTime local(QDateTime::currentDateTime());
        runTimestamp.appendChild(m_pDoc->createTextNode( local.toString(Qt::ISODate)));

        log.appendChild(habVersion);
        log.appendChild(rmVersion);
        log.appendChild(runTimestamp);
        log.appendChild(simulations);
        log.appendChild(messages);
        log.appendChild(meta);
        log.appendChild(status);

        QDomNode xmlNode = m_pDoc->createProcessingInstruction("xml",
                                                               "version=\"1.0\" encoding=\"ISO-8859-1\"");

        m_pDoc->appendChild(xmlNode);
        m_pDoc->appendChild(log);

    }
    m_xmlFile->close();
}

void XMLFile::AddRunMeta(QString sTagName, QString sTagValue){
    QDomElement meta_data, meta_data_tag;
    QDomElement documentElement = m_pDoc->documentElement();
    QDomNodeList elements = documentElement.elementsByTagName( "run_meta" );

    QDomText sMsgTxt = m_pDoc->createTextNode(sTagValue.toHtmlEscaped());

    if( elements.size() == 0 )
    {
        meta_data = m_pDoc->createElement( "run_meta" );
        documentElement.insertBefore( meta_data, documentElement );
    }
    else if( elements.size() == 1 )
    {
        meta_data = elements.at(0).toElement();
    }

    // Create the message itself
    meta_data_tag = m_pDoc->createElement( sTagName );
    meta_data_tag.appendChild(sMsgTxt);
    meta_data.appendChild( meta_data_tag );

    WriteDomToFile();
}

void XMLFile::WriteSimulationMeta(Simulation * logSim){
    QDomElement nodMeta, nodKeyVal;

    QDomElement sim_data = GetSimulationNode(logSim);
    nodMeta = m_pDoc->createElement( "sim_meta" );
    QHashIterator<QString, QString> i(logSim->GetMetaData());
    while (i.hasNext()) {
        i.next();

        nodKeyVal = m_pDoc->createElement( "meta" );
        nodKeyVal.setAttribute("key", i.key());
        nodKeyVal.appendChild(m_pDoc->createTextNode(i.value()));
        nodMeta.appendChild(nodKeyVal);
    }
    sim_data.appendChild(nodMeta);

    WriteDomToFile();
}

void XMLFile::WriteHistogram(RasterManager::HistogramsClass theHisto, Simulation * logSim){
    QDomElement nodHistogram, nodBin, nodArea, nodVol, nodCount;

    QDomElement sim_data = GetSimulationNode(logSim);

    if (theHisto.getNumBins() == 0)
        throw HabitatException(HISTOGRAM_ERROR, "Number of bins cannot be 0");
    int numBins = theHisto.getNumBins();
    double binSize = theHisto.getBinSize();
    double min = theHisto.getMinimumBin();
    double max = min + binSize;

    nodHistogram= m_pDoc->createElement( "histogram" );

    double * areaHistogram = theHisto.getAreaHistogram();
    double * volumeHistogram = theHisto.getVolumeHistogram();
    long * countHistogram = theHisto.getCountHistogram();

    for (int i=0; i < numBins; i++)
    {
        nodBin= m_pDoc->createElement( "bin" );
        nodBin.setAttribute("min", QString::number(min));
        nodBin.setAttribute("max", QString::number(max));

        nodArea = m_pDoc->createElement( "area" );
        nodArea.appendChild(m_pDoc->createTextNode(QString::number(areaHistogram[i])));
        nodBin.appendChild(nodArea);

        nodVol = m_pDoc->createElement( "volume" );
        nodVol.appendChild(m_pDoc->createTextNode(QString::number(volumeHistogram[i])));
        nodBin.appendChild(nodVol);

        nodCount = m_pDoc->createElement( "count" );
        nodCount.appendChild(m_pDoc->createTextNode(QString::number(countHistogram[i])));
        nodCount.setNodeValue(QString::number(countHistogram[i]));
        nodBin.appendChild(nodCount);

        nodHistogram.appendChild(nodBin);

        min = max;
        max += binSize;
    }

    sim_data.appendChild(nodHistogram);

    WriteDomToFile();
}

QDomElement XMLFile::GetSimulationNode(Simulation * logSim){
    int nSimID = logSim->GetID();
    QString simName = logSim->GetName();
    QDomElement report_data, sim_data;
    QDomElement documentElement = m_pDoc->documentElement();

    // First search for (create if necessary) a results.
    QDomNodeList reportEl = documentElement.elementsByTagName( "simulations" );

    if( reportEl.size() == 0 )
    {
        report_data = m_pDoc->createElement( "simulations" );
        documentElement.insertBefore( report_data, documentElement );
    }
    else if( reportEl.size() == 1 )
    {
        report_data = reportEl.at(0).toElement();
    }

    // Create the simulation node if it doesn't already exist
    QDomNodeList simEl = report_data.childNodes();

    bool bFound = false;
    for(int n= 0; n < simEl.length(); n++){
        QDomElement elSimulation = simEl.at(n).toElement();

        QString sSimID = QString::number(nSimID);
        QString simAttrID = elSimulation.attribute("id");

        if (elSimulation.hasAttribute("id") && simAttrID.compare(sSimID, Qt::CaseInsensitive) == 0){
            sim_data = elSimulation;
            bFound = true;
        }
    }

    if( !bFound )
    {
        sim_data = m_pDoc->createElement( "simulation" );
        sim_data.setAttribute("id", QString::number(nSimID));
        sim_data.setAttribute("name", simName);
        sim_data.setAttribute("type", logSim->GetType());
        report_data.appendChild(sim_data);
    }

    return sim_data;
}

void XMLFile::AddResult(Simulation * logSim, QString sTagName, QString sTagValue){
    QDomElement tagresult, report_data_tag, results_data;
    QDomText sMsgTxt = m_pDoc->createTextNode(sTagValue.toHtmlEscaped());

    QDomElement sim_data = GetSimulationNode(logSim);

    // First search for (create if necessary) a results.
    QDomNodeList reportEl = sim_data.elementsByTagName( "results" );

    if( reportEl.size() == 0 )
    {
        results_data = m_pDoc->createElement( "results" );
        sim_data.appendChild(results_data);
    }
    else if( reportEl.size() == 1 )
    {
        results_data = reportEl.at(0).toElement();
    }

    // Create the message itself
    tagresult = m_pDoc->createElement( sTagName );
    tagresult.appendChild(sMsgTxt);
    tagresult.appendChild( report_data_tag );
    results_data.appendChild(tagresult);

    WriteDomToFile();

}

QString XMLFile::GetTmpFileName()
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());


    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-");
    const int randomStringLength = 16; // assuming you want random strings of 12 characters

    QString randomString;

    for(int i=0; i < randomStringLength; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }

    return sTmpPrefix + randomString + ".xml";
}


void XMLFile::Log(QString sMsg, QString sException, int nSeverity, int indent)
{
    QDomElement messages, message, exception, description;
    QDomElement documentElement = m_pDoc->documentElement();
    QDomNodeList elements = documentElement.elementsByTagName( "messages" );

    QDomText sMsgTxt = m_pDoc->createTextNode(sMsg.toHtmlEscaped());
    QDomText sExceptionTxt = m_pDoc->createTextNode(sException.toHtmlEscaped());

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

    // Timestamps are useful
    QDateTime local(QDateTime::currentDateTime());
    message.setAttribute("timestamp", local.toString(Qt::ISODate));

    // Now create the description of the message
    description = m_pDoc->createElement( "description" );
    description.appendChild(sMsgTxt);
    message.appendChild( description );

    // Only create an exception if we need to.
    if (nSeverity != 0 && sException.compare("") != 0){
        exception = m_pDoc->createElement( "exception" );
        exception.appendChild(sExceptionTxt);
        message.appendChild( exception );
    }
    messages.appendChild( message );

    // If we're in debug mode output to screen
#ifdef QT_DEBUG
    qDebug() << qPrintable( QString("[%0]: [%1] %2")
                            .arg(enumToString((XML_LOG_SEVERITY)nSeverity))
                            .arg(local.toString(Qt::ISODate))
                            .arg(sMsg));
    if (nSeverity != 0 && sException.compare("") != 0){
        qDebug() << qPrintable( QString("---------------------[EXCEPTION]: \n%1\n----------------------------")
                                .arg(sException));
    }
#endif

    WriteDomToFile();
}


void XMLFile::QueueStatus(QString sID, StatusCode nCode, StatusType nType, int nTime)
{
    QDomElement statusEl, statusNode;
    QDomElement documentElement = m_pDoc->documentElement();
    QDomNodeList statusNL = documentElement.elementsByTagName( "status" );

    if( statusNL.size() == 0 )
    {
        statusEl = m_pDoc->createElement( "status" );
        documentElement.insertBefore( statusEl, documentElement );
    }
    else if( statusNL.size() == 1 )
    {
        statusEl = statusNL.at(0).toElement();
    }

    // Create the message itself
    statusNode = m_pDoc->createElement( enumToString( nType ) );
    statusNode.setAttribute( "value", enumToString( nCode ) );
    statusNode.setAttribute( "time", QString::number( nTime ) );
    statusNode.setAttribute( "id", sID );

    // Timestamps are useful
    QDateTime local(QDateTime::currentDateTime());
    statusNode.setAttribute("timestamp", local.toString(Qt::ISODate));

    statusEl.appendChild( statusNode );

    WriteDomToFile();
}

void XMLFile::AddStatus(QString sID, StatusCode nCode, StatusType nType, int nTime){
    QueueStatus(sID, nCode, nType, nTime);
    WriteDomToFile();
}

void XMLFile::WriteDomToFile(){

    // Great. Now Write the domelement to the file.
    // We're competing with the Desktop app for this file so we will try
    // a bunch of times to open it.
    int n = 0;
    while ( !m_xmlFile->open( QIODevice::WriteOnly | QIODevice::Text ) ){
        n++;
        if (n >= 100)
            throw HabitatException(FILE_WRITE_ERROR,  m_xmlFile->fileName());
        mySleep(50);
    }

    const int Indent = 4;

    QTextStream out(m_xmlFile);
    m_pDoc->save(out, Indent);
    m_xmlFile->close();
    CopyTmpToOutput();

}


}
