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
    inline NamedObject(QString sName){ m_sName = sName; }

    /**
     * @brief NamedObject
     * @param elObject a domelement containing the object's properties
     * @param sTitleName the name of the element containing the name of the object
     */
    inline NamedObject(QDomElement * elObject, QString sTitleName){
        m_sName = elObject->firstChildElement(sTitleName).text();
    }

    /**
     * @brief NamedObject (Copy Constructor)
     * @param obj
     */
    inline NamedObject(const NamedObject &obj){ m_sName = obj.GetName(); }

    /**
     * @brief operator =  (Assignment Operator)
     * @param obj
     */
    inline void operator=(const NamedObject &obj){ m_sName = obj.GetName(); }

    /**
     * @brief GetName
     * @return
     */
    inline QString GetName() const{ return m_sName; }

    /**
     * @brief SetName
     * @param sName
     */
    inline void SetName(QString sName){ m_sName = sName; }


protected:
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
    inline NamedObjectWithID(QString sName, int nID)
        : NamedObject(sName){
        m_id = nID;
    }

    /**
     * @brief NamedObjectWithID
     * @param elObject a domelement containing the object's properties
     * @param sTitleName the name of the element containing the name of the object
     * @param sIDName the name of the element containing the id of the object
     */
    inline NamedObjectWithID(QDomElement * elObject, QString sName, QString sIDName)
        : NamedObject(elObject, sName){
        m_id = elObject->firstChildElement(sIDName).text().toInt();
    }

    /**
     * @brief NamedObjectWithID (Copy Constructor)
     * @param obj
     */
    inline NamedObjectWithID(const NamedObjectWithID &obj)
        : NamedObject(obj){

    }

    /**
     * @brief operator = (Assignment Operator)
     * @param obj
     */
    inline void operator=(const NamedObjectWithID &obj){
        m_sName = obj.GetName();
        m_id = obj.GetID();
    }

    /**
     * @brief GetID
     * @return
     */
    inline int GetID() const { return m_id; }

    /**
     * @brief SetID
     * @param nID
     */
    inline void SetID(int nID){  m_id = nID; }

protected:
    int m_id; /**< TODO */

};
}


#endif // NAMEDOBJECT_H
