#ifndef PROJECTINPUTVECTOR_H
#define PROJECTINPUTVECTOR_H

#include <QString>
#include "project.h"
#include "rastermanager_interface.h"
#include "projectinput.h"

namespace HabitatModel {

class ProjectInput;

class ProjectInputVector : public ProjectInput
{
public:

    /**
     * @brief ProjectInputVector
     * @param source
     */
    ProjectInputVector(ProjectInputVector &source);


    void Init(QString sValueField);

    inline ~ProjectInputVector(){}

    /**
     * @brief Clone
     * @return
     */
    inline ProjectInput * Clone(){ return new ProjectInputVector(*this); }

    /**
     * @brief SetValueFieldName
     * @param sValueField
     */
    inline void SetValueFieldName(QString sValueField){ sValueField.append(""); return; }

    /**
     * @brief Prepare
     * @param TemplateRaster
     */
    void Prepare(Simulation * pSimulation);


    /**
     * @brief operator =
     * @param source
     */
    void operator =(ProjectInputVector &source);

    /**
     * @brief ProjectInputCSV
     * @param elProjectInput
     */
    ProjectInputVector(QDomElement *elProjectInput);

    // inline ~ProjectInputVector();

    /**
     * @brief getValueField
     * @return
     */
    QString getValueField();




private:
    QString m_sValueField;
};

}

#endif // PROJECTINPUTVECTOR_H
