#include "hsccategory.h"
#include <QDomElement>

namespace HabitatModel{

//<HSCCategories>
//  <CategoryID>1345</CategoryID>
//  <HSCID>36</HSCID>
//  <CategoryValue>7</CategoryValue>
//  <HSValue>1.2342352</HSValue>
//  <Description>A string</Description>
//</HSCCategories>

HSCCategory::HSCCategory(QDomElement * elCategory)
{
    m_nCategoryID = elCategory->firstChildElement("CategoryID").text().toInt();
    m_nCategoryValue = elCategory->firstChildElement("CategoryValue").text().toInt();
    m_fHSVal = elCategory->firstChildElement("HSValue").text().toDouble();
}

HSCCategory::HSCCategory(HSCCategory * pInput){
    m_nCategoryID = pInput->GetCategoryID();
    m_nCategoryValue = pInput->GetCategoryValue();
    m_fHSVal = pInput->GetHSValue();
}

}
