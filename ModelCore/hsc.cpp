#include "hsc.h"
#include "modelengine.h"

#include <QDomElement>

namespace HabitatModel{

HSC::HSC(const char * sName, int nID) : NamedObjectWithID(sName, nID)
{
}

HSC::HSC(QDomElement * elHSC) : NamedObjectWithID(elHSC, "Title", "HSCID")
{

    //Example Node:
    //<HSC>
    //  <HSCID>1</HSCID>
    //  <HSCName>Passage depth at velocities less than 61 cm/s for Chinook (Sautner , 1984)</HSCName>
    //  <SpeciesID>28</SpeciesID>
    //  <LifeStageID>42</LifeStageID>
    //  <HSCVariableID>21</HSCVariableID>
    //  <UnitID>4</UnitID>
    //  <SourceID>1</SourceID>
    //  <Figure>2</Figure>
    //  <Location>Generic</Location>
    //  <CategoryID>79</CategoryID>
    //</HSC>

    m_location = elHSC->firstChildElement("VariableName").text();
    m_figure = elHSC->firstChildElement("CategoryID").text().toInt();

    m_source = ModelEngine::GetLookupTableItem(elHSC, "SourceID");
    m_lifestage = ModelEngine::GetLookupTableItem(elHSC, "LifestageID");
    m_species = ModelEngine::GetLookupTableItem(elHSC, "SpeciesID");

    m_variable = ModelEngine::GetVariable(elHSC, "HSCVariableID");
    m_unit = ModelEngine::GetUnit(elHSC, "HSCVariableID");

    LoadCategories();
    LoadCoordinatePairs();
}

void HSC::LoadCategories(){

//    QDomNodeList elvars = m_elConfig.elementsByTagName("Variables");

//    for(int n= 0; n < elvars.length(); n++){
//        QDomNode elvar = elvars.at(n);
//        int nvarID = elvar.firstChildElement("VariableID").text().toInt();
//        QString sname = elvar.firstChildElement("VariableName").text();
//        int ncatid = elvar.firstChildElement("CategoryID").text().toInt();
//        int ndimensionid = elvar.firstChildElement("DimensionID").text().toInt();

//        m_hmvariable_store.insert(nvarID, new HMVariable(sname.toStdString().c_str(), nvarID, m_lookup_table.value(ndimensionid), m_lookup_table.value(ncatid)));
//    }

//    m_categories.insert();
}
void HSC::LoadCoordinatePairs(){

//    QDomNodeList elvars = m_elConfig.elementsByTagName("Variables");

//    for(int n= 0; n < elvars.length(); n++){
//        QDomNode elvar = elvars.at(n);
//        int nvarID = elvar.firstChildElement("VariableID").text().toInt();
//        QString sname = elvar.firstChildElement("VariableName").text();
//        int ncatid = elvar.firstChildElement("CategoryID").text().toInt();
//        int ndimensionid = elvar.firstChildElement("DimensionID").text().toInt();

//        m_hmvariable_store.insert(nvarID, new HMVariable(sname.toStdString().c_str(), nvarID, m_lookup_table.value(ndimensionid), m_lookup_table.value(ncatid)));
//    }

//    m_coordinate_pairs.insert();
}

}
