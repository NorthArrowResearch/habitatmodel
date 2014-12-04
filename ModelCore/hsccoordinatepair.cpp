#include "hsccoordinatepair.h"
#include <QDomElement>

namespace HabitatModel{

HSCCoordinatePair::HSCCoordinatePair(QDomElement * elCoordinatePair)
{

    // <HSCCoordinatePairs>
    // <CoordinatePairID>11</CoordinatePairID>
    // <HSCID>3</HSCID>
    // <XValue>7</XValue>
    // <HSValue>1</HSValue>
    // </HSCCoordinatePairs>

    m_fXVal = elCoordinatePair->firstChildElement("XValue").text().toDouble();
    m_fHSVal = elCoordinatePair->firstChildElement("HSValue").text().toDouble();
}

HSCCoordinatePair::HSCCoordinatePair(HSCCoordinatePair * pInput){
    m_fHSVal = pInput->GetHSValue();
    m_fXVal = pInput->GetXValue();
}

HSCCoordinatePair::~HSCCoordinatePair(){

}

}
