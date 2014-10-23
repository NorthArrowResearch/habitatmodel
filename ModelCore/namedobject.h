#ifndef NAMEDOBJECT_H
#define NAMEDOBJECT_H

#include <QString>
#include <QDomElement>

namespace HabitatModel {

/**
 * @brief The NamedObject class
 */
class NamedObject
{
public:

    /**
     * @brief NamedObject
     * @param sName
     */
    NamedObject(const char * sName);

    /**
     * @brief NamedObject
     * @param elObject a domelement containing the object's properties
     * @param sTitleName the name of the element containing the name of the object
     */
    NamedObject(QDomElement * elObject, QString sTitleName);

    /**
     * @brief NamedObject (Copy Constructor)
     * @param obj
     */
    NamedObject(const NamedObject &obj);

    /**
     * @brief operator =  (Assignment Operator)
     * @param obj
     */
    void operator=(const NamedObject &obj);

    /**
     * @brief GetName
     * @return
     */
    QString GetName() const;

    /**
     * @brief SetName
     * @param sName
     */
    void SetName(const char * sName);


private:
    QString m_sName; /**< TODO */
};

/**
 * @brief The NamedObjectWithID class
 */
class NamedObjectWithID : public NamedObject
{
public:

    /**
     * @brief NamedObjectWithID
     * @param sName
     * @param nID
     */
    NamedObjectWithID(const char * sName, int nID);

    /**
     * @brief NamedObjectWithID
     * @param elObject a domelement containing the object's properties
     * @param sTitleName the name of the element containing the name of the object
     * @param sIDName the name of the element containing the id of the object
     */
    NamedObjectWithID(QDomElement * elObject, QString sTitleName, QString sIDName);

    /**
     * @brief NamedObjectWithID (Copy Constructor)
     * @param obj
     */
    NamedObjectWithID(const NamedObject &obj);

    /**
     * @brief operator = (Assignment Operator)
     * @param obj
     */
    void operator=(const NamedObject &obj);

    /**
     * @brief GetID
     * @return
     */
    int GetID() const;

    /**
     * @brief SetID
     * @param nID
     */
    void SetID(int nID);

private:
    int m_id; /**< TODO */

};
}


#endif // NAMEDOBJECT_H
