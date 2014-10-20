#include "namedobject.h"

namespace HabitatModel {

NamedObject::NamedObject(const char * sName)
{
    m_sName = sName;
}

NamedObject::NamedObject(const NamedObject &obj)
{
    m_sName = obj.GetName();
}

void NamedObject::operator =(const NamedObject &obj)
{
    m_sName = obj.GetName();
}

std::string NamedObject::GetName() const
{
    return m_sName;
}


NamedObjectWithID::NamedObjectWithID(const char * sName, int nID){
    id = nID;
    NamedObject(sName);
}

int NamedObjectWithID::GetID() const
{
    return id;
}


}
