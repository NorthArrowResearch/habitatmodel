#include "hsisimulation.h"

#include <QDomElement>
#include "hsi.h"
#include "modelengine.h"

namespace HabitatModel{


HSISimulation::HSISimulation(QDomElement *elSimulation)
    : Simulation(elSimulation)
{

    // Now Create our HSI object if there is one.
    QDomElement elHSI = ModelEngine::GetConfig()->firstChildElement("HSI");
    if (!elHSI.isNull() && elHSI.firstChildElement("HSIID").text().toInt() == this->GetID()){
        m_hsiRef = new HSI(&elHSI);
    }

    //QString stupid = HabitatModel::ModelEngine::GetVariable(myInt)->GetName();

}

}
