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
     * @param sName
     * @param nID
     * @param psValueField
     */
    ProjectInputVector(const char *sName, int nID, const char * psValueField);

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

private:
    QString m_sValueField;
};

}

#endif // PROJECTINPUTVECTOR_H
