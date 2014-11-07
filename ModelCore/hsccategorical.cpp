#include "hsccategorical.h"
#include "hsccategory.h"
#include "project.h"
#include <QDomElement>
#include <QHashIterator>

namespace HabitatModel {

HSCCategorical::HSCCategorical(QDomElement * elHSC)
    : HSC(elHSC)
{ }

HSCCategorical::~HSCCategorical()
{
    // Empty the category store
    QHashIterator<int, HSCCategory *> m(m_categories);
    while (m.hasNext()) {
        m.next();
        delete m.value();
    }
}


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
