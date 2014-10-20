#ifndef XMLFILEHANDER_H
#define XMLFILEHANDER_H

#include <QString>
#include <QDomDocument>
#include <QFile>

class QDir;

namespace GCD {
namespace Project {

/**
 * @brief
 *
 */
class XMLFileHander
{
public:
    /**
     * @brief
     *
     */
    XMLFileHander();
    /**
     * @brief
     *
     */
    ~XMLFileHander();
    /**
     * @brief
     *
     * @param sFilePath
     */
    void Load(QString & sFilePath);
    /**
     * @brief
     *
     * @param sXPath
     * @param sResult
     * @param xParentNode
     * @return bool
     */
    bool GetNodeString(QString sXPath, QString & sResult, QDomElement & xParentNode);
    /**
     * @brief
     *
     * @param sXPath
     * @param dResult
     * @param xParentNode
     * @return bool
     */
    bool GetNodeDateTime(QString sXPath, QDate & dResult, QDomElement & xParentNode);
    /**
     * @brief
     *
     * @param sXPath
     * @param nPrecision
     * @param xParentNode
     * @return bool
     */
    bool GetNodeInteger(QString sXPath, int & nPrecision, QDomElement & xParentNode);
    /**
     * @brief
     *
     * @param sXPath
     * @param qResult
     * @param xParentNode
     * @return bool
     */
    bool GetNodeFolder(QString sXPath, QDir & qResult, QDomElement & xParentNode);
    /**
     * @brief
     *
     * @param sXPath
     * @param fValue
     * @param xParentNode
     * @return bool
     */
    bool GetNodeDouble(QString sXPath, double & fValue, QDomElement & xParentNode);

    /**
     * @brief
     *
     * @return QDomDocument
     */
    QDomDocument * Document();

private:
    QString m_sFilePath; /**< TODO */

    QDomDocument * m_pDoc; /**< TODO */
    QFile * m_pXMLFile; /**< TODO */

};

}
}

#endif // XMLFILEHANDER_H
