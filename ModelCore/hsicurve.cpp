#include "hsicurve.h"
#include "project.h"

namespace HabitatModel{


HSICurve::HSICurve(QDomElement * elHSICurve)
{
    m_fWeight = elHSICurve->firstChildElement("Weight").text().toDouble();
    m_hsc = Project::GetHSC(elHSICurve, "HSCID");
}


HSICurve::~HSICurve(){


}

}

//    Example Node:
//    <HSICurves>
//    <HSICurveID>3</HSICurveID>
//    <HSIID>2</HSIID>
//    <HSCID>60</HSCID>
//    <Weight>0.5</Weight>
//    </HSICurves>
