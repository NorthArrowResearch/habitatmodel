#include "hsi.h"

namespace HabitatModel{

HSI::HSI(const char *sName) : NamedObject(sName)
{
}

HSI::HSI(QDomElement *elHSI, QDomElement *elConfig) : NamedObject(elHSI, "Title")
{
//    m_lifestage;
//    m_species;
//    m_method;
// HSICurve
}


}
