#include "hsccoordinatepair.h"
#include "project.h"

namespace HabitatModel{

HSCCoordinatePair::HSCCoordinatePair(QDomElement * elCoordinatePair)
{

    //<HSCCoordinatePairs>
    //<CoordinatePairID>11</CoordinatePairID>
    //<HSCID>3</HSCID>
    //<XValue>7</XValue>
    //<HSValue>1</HSValue>
    //</HSCCoordinatePairs>

    m_HSC = Project::GetHSC(elCoordinatePair, "HSCID");
    m_nXVal = elCoordinatePair->firstChildElement("XValue").text().toInt();
    m_nHSVal = elCoordinatePair->firstChildElement("HSValue").text().toInt();
}



}
