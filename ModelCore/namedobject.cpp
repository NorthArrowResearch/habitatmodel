#include "namedobject.h"
#include <QString>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QDomElement>

namespace HabitatModel {

/*************************************************************

            NAMED OBJECT: A generalized and highly useful object

*************************************************************/

NamedObject::NamedObject(const char * sName)
{
    m_sName = sName;
}

NamedObject::NamedObject(QDomElement * elObject, QString sTitleName)
{
    m_sName = elObject->firstChildElement(sTitleName).text();
}

NamedObject::NamedObject(const NamedObject &obj)
{
    m_sName = obj.GetName();
}

void NamedObject::operator =(const NamedObject &obj)
{
    m_sName = obj.GetName();
}

QString NamedObject::GetName() const
{
    return m_sName;
}

/*************************************************************

            NAMED OBJECT WTIH ID: same as above. Now with id

*************************************************************/


NamedObjectWithID::NamedObjectWithID(const char * sName, int nID) : NamedObject(sName)
{
    m_id = nID;
}

NamedObjectWithID::NamedObjectWithID(QDomElement * elObject, QString sTitleName, QString sIDName) : NamedObject(elObject, sTitleName)
{
    m_id = elObject->firstChildElement(sIDName).text().toInt();
}


int NamedObjectWithID::GetID() const
{
    return m_id;
}


}
