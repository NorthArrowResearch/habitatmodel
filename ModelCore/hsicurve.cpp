#include "hsicurve.h"
#include "modelengine.h"

namespace HabitatModel{


HSICurve::HSICurve(QDomElement * elHSICurve)
{
    m_weight = elHSICurve->firstChildElement("Weight").text().toDouble();
    m_hsc = ModelEngine::GetHSC(elHSICurve, "HSIID");
}

}

//    Example Node:
//    <HSICurves>
//    <HSICurveID>3</HSICurveID>
//    <HSIID>2</HSIID>
//    <HSCID>60</HSCID>
//    <Weight>0.5</Weight>
//    </HSICurves>
