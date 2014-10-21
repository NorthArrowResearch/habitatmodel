#ifndef NAMEDOBJECT_H
#define NAMEDOBJECT_H

#include <QString>
#include <QDomElement>

namespace HabitatModel {

/**
 * @brief
 *
 */
class NamedObject
{
public:
    /**
     * @brief
     *
     * @param sName
     */
    NamedObject(const char * sName);

    /**
     * @brief NamedObject
     * @param elObject a domelement containing the object's properties
     * @param sTitleName the name of the element containing the name of the object
     */
    NamedObject(QDomElement * elObject, QString sTitleName);

    // Copy Constructor
    /**
     * @brief
     *
     * @param obj
     */
    NamedObject(const NamedObject &obj);

    // Assignment Operator
    /**
     * @brief
     *
     * @param obj
     */
    void operator=(const NamedObject &obj);

    /**
     * @brief
     *
     * @return std::string
     */
    QString GetName() const;
    /**
     * @brief
     *
     * @param sName
     */
    void SetName(const char * sName);


private:
    QString m_sName; /**< TODO */
};

/**
 * @brief
 *
 */
class NamedObjectWithID : public NamedObject
{
public:
    /**
     * @brief
     *
     * @param sName
     */
    NamedObjectWithID(const char * sName, int nID);
    /**
     * @brief NamedObject
     * @param elObject a domelement containing the object's properties
     * @param sTitleName the name of the element containing the name of the object
     * @param sIDName the name of the element containing the id of the object
     */
    NamedObjectWithID(QDomElement * elObject, QString sTitleName, QString sIDName);
    // Copy Constructor
    /**
     * @brief
     *
     * @param obj
     */
    NamedObjectWithID(const NamedObject &obj);

    // Assignment Operator
    /**
     * @brief
     *
     * @param obj
     */
    void operator=(const NamedObject &obj);

    /**
     * @brief
     *
     * @return std::string
     */
    int GetID() const;
    /**
     * @brief
     *
     * @param sName
     */
    void SetID(int nID);

private:
    int m_id; /**< TODO */

};
}


#endif // NAMEDOBJECT_H
