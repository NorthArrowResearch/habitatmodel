#include <QDomElement>

#include "hsi.h"
#include "hsicurve.h"
#include "modelengine.h"

namespace HabitatModel{

HSI::HSI(QDomElement * elHSI) : NamedObject(elHSI, "Title")
{

    //    Example Node:
    //    <HSI>
    //    <HSIID>2</HSIID>
    //    <Title>Maret et al 2006 Upper Salmon - vel, depth - Chinook Juvenile</Title>
    //    <SpeciesID>28</SpeciesID>
    //    <LifestageID>40</LifestageID>
    //    <HSIMethodID>46</HSIMethodID>
    //    </HSI>

    m_species = ModelEngine::GetLookupTableItem(elHSI, "SpeciesID");
    m_lifestage = ModelEngine::GetLookupTableItem(elHSI, "LifestageID");
    m_method = ModelEngine::GetLookupTableItem(elHSI, "HSIMethodID");

    LoadCurves();
}

void HSI::LoadCurves(){

    QDomNodeList elHSICurves = ModelEngine::GetConfig()->elementsByTagName("HSICurves");

    for(int n= 0; n < elHSICurves.length(); n++){
        QDomElement elCurve = elHSICurves.at(n).toElement();
        int nCurveID = elCurve.firstChildElement("HSICurveID").text().toInt();
        m_curves.insert(nCurveID, new HSICurve(&elCurve));
    }

}


}
