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

    //    Example Node:
    //    <HSICurves>
    //    <HSICurveID>3</HSICurveID>
    //    <HSIID>2</HSIID>
    //    <HSCID>60</HSCID>
    //    <Weight>0.5</Weight>
    //    </HSICurves>

//    for(int n= 0; n < elListItems.length(); n++){
//        QDomNode elListItem = elListItems.at(n);
//        int nListItemID = elListItem.firstChildElement("HSICurveID").text().toInt();

//        //        NamedObjectWithID * dimensionID = GetLookupTableItem(&elUnit,"DimensionID");

//        m_curves.insert(nListItemID, new NamedObjectWithID(sname.toStdString().c_str(), nListItemID));
//    }

    //    m_curves.insert(new HSICurve());
}


}
