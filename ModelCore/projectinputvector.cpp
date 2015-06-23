#include "projectinputvector.h"
#include "project.h"
#include "rastermanager_interface.h"

namespace HabitatModel {

ProjectInputVector::ProjectInputVector(ProjectInputVector &source): ProjectInput(source)
{

}

ProjectInputVector::ProjectInputVector(QDomElement * elProjectInput)
    : ProjectInput(elProjectInput)
{

//    m_sXField = QString(psXField);
//    m_sYField = QString(psYField);
//    m_sValueField = QString(psValueField);

}

void ProjectInputVector::Init(QString sValueField){
     m_sValueField = sValueField;
}

void ProjectInputVector::Prepare(Simulation * pSimulation){
    pSimulation->GetRasterExtentMeta();
}

QString ProjectInputVector::getValueField()
{
    return m_sValueField;
}

}
