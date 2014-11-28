#ifndef PROJECTINPUTVECTOR_H
#define PROJECTINPUTVECTOR_H

#include <QString>
#include "projectinput.h"

namespace HabitatModel {

class ProjectInputVector : public ProjectInput
{
public:


    inline ~ProjectInputVector(){}

    /**
     * @brief ProjectInputVector
     * @param source
     */
    ProjectInputVector(ProjectInputVector &source);

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
     * @brief Prepare
     * @param TemplateRaster
     */
    void Prepare(RasterManager::RasterMeta * TemplateRaster, QString NewPath);

    void Init(QString sValueField);
private:
    QString m_sValueField;
};

}

#endif // PROJECTINPUTVECTOR_H
