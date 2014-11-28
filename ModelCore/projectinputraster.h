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
     * @param source
     */
    ProjectInputRaster(const ProjectInputRaster & source);

    inline ~ProjectInputRaster(){}

    /**
     * @brief operator =
     * @param source
     */
    void operator =(ProjectInputRaster &source);

    inline ProjectInput * Clone(){ return new ProjectInputRaster(*this); }

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
