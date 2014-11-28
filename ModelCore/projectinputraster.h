#ifndef PROJECTINPUTRASTER_H
#define PROJECTINPUTRASTER_H

#include "projectinput.h"

namespace HabitatModel {

class ProjectInput;

class ProjectInputRaster : public ProjectInput
{
public:

    /**
     * @brief ProjectInputRaster
     * @param sName
     * @param nID
     */
    ProjectInputRaster(const char *sName, int nID);

    inline ~ProjectInputRaster(){}

    /**
     * @brief ProjectInputRaster
     * @param source
     */
    ProjectInputRaster(ProjectInputRaster &source);

    /**
     * @brief operator =
     * @param source
     */
    void operator =(ProjectInputRaster &source);

    /**
     * @brief ProjectInputCSV
     * @param elProjectInput
     */
    ProjectInputRaster(QDomElement *elProjectInput);

    void Prepare(RasterManager::RasterMeta *TemplateRaster, QString NewPath);
    void Init();
};

}

#endif // PROJECTINPUTRASTER_H
