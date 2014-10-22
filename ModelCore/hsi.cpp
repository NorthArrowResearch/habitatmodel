#include <QDomElement>

#include "hsi.h"
#include "hsicurve.h"
#include "modelengine.h"

namespace HabitatModel{

HSI::HSI(const char * sName) : NamedObject(sName)
{
}

HSI::HSI(QDomElement * elHSI) : NamedObject(elHSI, "Title")
{
    m_species = ModelEngine::GetLookupTableItem(elHSI, "SpeciesID");
    m_lifestage = ModelEngine::GetLookupTableItem(elHSI, "LifestageID");
    m_method = ModelEngine::GetLookupTableItem(elHSI, "HSIMethodID");

    LoadCurves();
}

void HSI::LoadCurves(){

//    m_curves.insert(new HSICurve());
}


}
