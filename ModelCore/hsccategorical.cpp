#include "hsccategorical.h"

#include "hsccategory.h"
#include "project.h"
#include <QDomElement>
#include <QHashIterator>

namespace HabitatModel {

HSCCategorical::HSCCategorical(QDomElement * elHSC)
    : HSC(elHSC)
{
    LoadCategories();
}

HSCCategorical::~HSCCategorical()
{
    // Empty the category store
    QHashIterator<int, HSCCategory *> m(m_categories);
    while (m.hasNext()) {
        m.next();
        delete m.value();
    }
}

void HSCCategorical::LoadCategories(){

//    TODO: I don't have an example for the structure of this

//    QDomNodeList elCategory = Project::GetConfig()->elementsByTagName("HSCCategories");

//    for(int n= 0; n < elCategory.length(); n++){
//        QDomElement elCategory = elCoordPairs.at(n).toElement();
//        int nCategory = elCategory.firstChildElement("HSCCategoriesID").text().toInt();
//        m_categories.insert(nCategory, new HSCCoordinatePair(elCategory));
//    }

}

/* Look up the cateogry and retrieve the associated habitat suitability value.
 * Note that the argument nCategory refers to a category *value*. i.e. the
 * value in a raster, CSV or ShapeFile. This is different than the integer
 * key of the hash dictionary which refers to the model configuration XML
 * database ID for the category.
 */
double HSCCategorical::GetHSValue(int nCategory)
{
    QHashIterator<int, HSCCategory *> m(m_categories);
    while (m.hasNext()) {
        m.next();

        if (m.value()->GetCategory() == nCategory)
            return m.value()->GetHSValue();
    }

    return 0;
}

} // HabitatModel
