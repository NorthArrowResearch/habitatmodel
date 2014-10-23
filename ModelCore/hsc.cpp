#include "hsc.h"
#include "project.h"
#include "hsccoordinatepair.h"

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

    m_source = Project::GetLookupTableItem(elHSC, "SourceID");
    m_lifestage = Project::GetLookupTableItem(elHSC, "LifestageID");
    m_species = Project::GetLookupTableItem(elHSC, "SpeciesID");

    m_variable = Project::GetVariable(elHSC, "HSCVariableID");
    m_unit = Project::GetUnit(elHSC, "HSCVariableID");

    LoadCategories();
    LoadCoordinatePairs();
}

void HSC::LoadCategories(){

//    TODO: I don't have an example for the structure of this

//    QDomNodeList elCategory = Project::GetConfig()->elementsByTagName("HSCCategories");

//    for(int n= 0; n < elCategory.length(); n++){
//        QDomElement elCategory = elCoordPairs.at(n).toElement();
//        int nCategory = elCategory.firstChildElement("HSCCategoriesID").text().toInt();
//        m_categories.insert(nCategory, new HSCCoordinatePair(elCategory));
//    }

}
void HSC::LoadCoordinatePairs(){

    QDomNodeList elCoordPairs = Project::GetConfig()->elementsByTagName("HSCCoordinatePairs");

    for(int n= 0; n < elCoordPairs.length(); n++){
        QDomElement elCoordinatePair = elCoordPairs.at(n).toElement();
        int nCoordinatePairID = elCoordinatePair.firstChildElement("CoordinatePairID").text().toInt();
        m_coordinate_pairs.insert(nCoordinatePairID, new HSCCoordinatePair(&elCoordinatePair));
    }

}

}
