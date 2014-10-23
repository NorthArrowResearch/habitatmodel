#include <QDomElement>

#include "hsi.h"
#include "hsicurve.h"
#include "project.h"

namespace HabitatModel{

HSI::HSI(QDomElement * elHSI) : NamedObjectWithID(elHSI, "Title", "HSIID")
{

    //    Example Node:
    //    <HSI>
    //    <HSIID>2</HSIID>
    //    <Title>Maret et al 2006 Upper Salmon - vel, depth - Chinook Juvenile</Title>
    //    <SpeciesID>28</SpeciesID>
    //    <LifestageID>40</LifestageID>
    //    <HSIMethodID>46</HSIMethodID>
    //    </HSI>

    m_species = Project::GetLookupTableItem(elHSI, "SpeciesID");
    m_lifestage = Project::GetLookupTableItem(elHSI, "LifestageID");
    m_method = Project::GetLookupTableItem(elHSI, "HSIMethodID");

    LoadCurves();
}

void HSI::LoadCurves(){

    QDomNodeList elHSICurves = Project::GetConfig()->elementsByTagName("HSICurves");

    for(int n= 0; n < elHSICurves.length(); n++){
        QDomElement elCurve = elHSICurves.at(n).toElement();

        // Only add a curve if it belongs to THIS HSI.
        int nHSIID = elCurve.firstChildElement("HSIID").text().toInt();
        if (nHSIID == this->GetID()){
            int nCurveID = elCurve.firstChildElement("HSICurveID").text().toInt();
            m_curves.insert(nCurveID, new HSICurve(&elCurve));
        }
    }

}


HSICurve * HSI::GetCurve(int nHSICurveID){
    return m_curves.value(nHSICurveID);
}

}
