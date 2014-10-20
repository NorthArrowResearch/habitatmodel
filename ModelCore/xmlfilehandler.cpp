#include "xmlfilehandler.h"

#include <QDate>
#include <QDomElement>
#include <QDir>

namespace GCD {
namespace Project {

XMLFileHander::XMLFileHander()
{
}

XMLFileHander::~XMLFileHander()
{
    if (!m_pXMLFile == NULL)
    {
        if (m_pXMLFile->isOpen())
            m_pXMLFile->close();

        delete m_pXMLFile;
    }

    if (!m_pDoc == NULL)
        delete m_pDoc ;
}

QDomDocument * XMLFileHander::Document()
{
    return m_pDoc;
}

void XMLFileHander::Load(QString &sFilePath)
{
    if (sFilePath.isEmpty() || sFilePath.isNull())
        throw "The file path is null or empty";
    else
        if (!QFile::exists(sFilePath))
            throw "The GCD project file does not exist.";

    m_pDoc = new QDomDocument;
    m_pXMLFile = new QFile(sFilePath);
    if (!m_pXMLFile->open(QIODevice::ReadOnly))
        throw "Failed to open the GCD XML project file as read only.";

    if (!m_pDoc->setContent(m_pXMLFile->readAll())) {
        m_pXMLFile->close();
        throw "Failed to load DOM from GCD XML project file.";
    }
}

bool XMLFileHander::GetNodeString(QString sXPath, QString & sResult, QDomElement & xParentNode)
{
    if (sXPath.isNull() || sXPath.isEmpty())
        throw "The XPath cannot be null or empty.";

    //if (xParentNode == NULL)
    //    xParentNode = &(m_pDoc->documentElement());

    // Remove any initial slash
    if (sXPath.startsWith("/"))
        sXPath = sXPath.right(sXPath.length()-1);

    int nFirstSlash = sXPath.indexOf("/");
    if (nFirstSlash >= 0)
    {
        QString sParentNode = sXPath.left(nFirstSlash);
        QDomElement aNode = xParentNode.firstChildElement(sParentNode);
        if (aNode.isNull())
            return false;
        else
        {
            sXPath = sXPath.right(sXPath.length() - nFirstSlash);
            return GetNodeString(sXPath, sResult, aNode);
        }
    }
    else
    {
        if (xParentNode.isNull())
            return false;
        else
        {
            QDomElement aNode = xParentNode.firstChildElement(sXPath);
            if (aNode.isNull())
                return false;
            else
            {
                sResult = aNode.text();
                return !(sResult.isNull() || sResult.isEmpty());
            }
        }
    }
}

bool XMLFileHander::GetNodeFolder(QString sXPath, QDir & qResult, QDomElement & xParentNode)
{
    if (sXPath.isNull() || sXPath.isEmpty())
        throw "The XPath cannot be null or empty.";

    //if (xParentNode == NULL)
    //    xParentNode = &(m_pDoc->documentElement());

    // Remove any initial slash
    if (sXPath.startsWith("/"))
        sXPath = sXPath.right(sXPath.length()-1);

    int nFirstSlash = sXPath.indexOf("/");
    if (nFirstSlash >= 0)
    {
        QString sParentNode = sXPath.left(nFirstSlash);
        QDomElement aNode = xParentNode.firstChildElement(sParentNode);
        if (aNode.isNull())
            return false;
        else
        {
            sXPath = sXPath.right(sXPath.length() - nFirstSlash);
            return GetNodeFolder(sXPath, qResult, aNode);
        }
    }
    else
    {
        if (xParentNode.isNull())
            return false;
        else
        {
            QDomElement aNode = xParentNode.firstChildElement(sXPath);
            if (aNode.isNull())
                return false;
            else
            {
               qResult = QDir(aNode.text());
                return qResult.exists();
            }
        }
    }
}

bool XMLFileHander::GetNodeInteger(QString sXPath, int & nPrecision, QDomElement & xParentNode)
{
    if (sXPath.isNull() || sXPath.isEmpty())
        throw "The XPath cannot be null or empty.";

    //if (xParentNode == NULL)
    //    xParentNode = &(m_pDoc->documentElement());

    // Remove any initial slash
    if (sXPath.startsWith("/"))
        sXPath = sXPath.right(sXPath.length()-1);

    int nFirstSlash = sXPath.indexOf("/");
    if (nFirstSlash >= 0)
    {
        QString sParentNode = sXPath.left(nFirstSlash);
        QDomElement aNode = xParentNode.firstChildElement(sParentNode);
        if (aNode.isNull())
            return false;
        else
        {
            sXPath = sXPath.right(sXPath.length() - nFirstSlash);
            return GetNodeInteger(sXPath, nPrecision, aNode);
        }
    }
    else
    {
        if (xParentNode.isNull())
            return false;
        else
        {
            QDomElement aNode = xParentNode.firstChildElement(sXPath);
            if (aNode.isNull())
                return false;
            else
            {
                try
                {
                    nPrecision = atoi(aNode.text().toStdString().c_str());
                }
                catch (std::exception& ex)
                {
                    throw "Failed to convert the GCD project precision XML node value to an integer.";
                }

                return (nPrecision >= 0);
            }
        }
    }
}

bool XMLFileHander::GetNodeDateTime(QString sXPath, QDate & dResult, QDomElement & xParentNode)
{
    QString sDate;
    if (!GetNodeString(sXPath, sDate, xParentNode))
        return false;

    dResult = QDate::fromString(sDate,"yyyy/MM/dd");

    if (!dResult.isValid() || dResult.isNull())
        dResult = QDate::fromString(sDate,Qt::ISODate);

    return (dResult.isValid() && !dResult.isNull());
}


bool XMLFileHander::GetNodeDouble(QString sXPath, double & fValue, QDomElement & xParentNode)
{
    if (sXPath.isNull() || sXPath.isEmpty())
        throw "The XPath cannot be null or empty.";

    // Remove any initial slash
    if (sXPath.startsWith("/"))
        sXPath = sXPath.right(sXPath.length()-1);

    int nFirstSlash = sXPath.indexOf("/");
    if (nFirstSlash >= 0)
    {
        QString sParentNode = sXPath.left(nFirstSlash);
        QDomElement aNode = xParentNode.firstChildElement(sParentNode);
        if (aNode.isNull())
            return false;
        else
        {
            sXPath = sXPath.right(sXPath.length() - nFirstSlash);
            return GetNodeDouble(sXPath, fValue, aNode);
        }
    }
    else
    {
        if (xParentNode.isNull())
            return false;
        else
        {
            QDomElement aNode = xParentNode.firstChildElement(sXPath);
            if (aNode.isNull())
                return false;
            else
            {
                try
                {
                    fValue = atof(aNode.text().toStdString().c_str());
                }
                catch (std::exception& ex)
                {
                    throw "Failed to convert the XML node value to a double.";
                }

                return (fValue >= 0);
            }
        }
    }
}

}
}
