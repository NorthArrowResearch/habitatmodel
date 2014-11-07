#include "hsccoordinatepair.h"
#include "project.h"

namespace HabitatModel{

HSCCoordinatePair::HSCCoordinatePair(QDomElement * elCoordinatePair)
{

    // <HSCCoordinatePairs>
    // <CoordinatePairID>11</CoordinatePairID>
    // <HSCID>3</HSCID>
    // <XValue>7</XValue>
    // <HSValue>1</HSValue>
    // </HSCCoordinatePairs>

    m_HSC = Project::GetHSC(elCoordinatePair, "HSCID");
    m_fXVal = elCoordinatePair->firstChildElement("XValue").text().toDouble();
    m_fHSVal = elCoordinatePair->firstChildElement("HSValue").text().toDouble();
}


}
