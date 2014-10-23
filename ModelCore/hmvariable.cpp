#include "hmvariable.h"
#include "project.h"

namespace HabitatModel{


HMVariable::HMVariable(QDomElement * elHSC) : NamedObjectWithID(elHSC, "VariableName", "VariableID")
{
    //Sample Input
    //<Variables>
    //  <VariableID>1</VariableID>
    //  <VariableName>Average ph</VariableName>
    //  <CategoryID>35</CategoryID>
    //  <DimensionID>8</DimensionID>
    //</Variables>

    m_dimension = Project::GetLookupTableItem(elHSC,"DimensionID");
    m_category = Project::GetLookupTableItem(elHSC,"DimensionID");
}


NamedObjectWithID * HMVariable::getDimentsion()
{
    return m_dimension;
}

NamedObjectWithID * HMVariable::getCategory()
{
    return m_category;
}

}
