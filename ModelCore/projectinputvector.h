#ifndef PROJECTINPUTVECTOR_H
#define PROJECTINPUTVECTOR_H

#include <QString>
#include "projectinput.h"

namespace HabitatModel {

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
     * @brief operator =
     * @param source
     */
    void operator =(ProjectInputVector &source);

    inline ProjectInput * Clone(){ return new ProjectInputVector(*this); }

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

    /**
     * @brief SetValueFieldName
     * @param sValueField
     */
    inline void SetValueFieldName(QString sValueField){ return; }

    /**
     * @brief Prepare
     * @param TemplateRaster
     */
    void Prepare(RasterManager::RasterMeta * TemplateRaster);


private:
    QString m_sValueField;
};

}

#endif // PROJECTINPUTVECTOR_H
