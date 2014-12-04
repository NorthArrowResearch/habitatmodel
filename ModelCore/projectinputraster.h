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

    inline ProjectInput * Clone(){ return new ProjectInputRaster(*this); }

    void Prepare(RasterManager::RasterMeta *TemplateRasterMeta);

    inline void SetValueFieldName(QString sValueField){ return; }

    /**
     * @brief ProjectInputCSV
     * @param elProjectInput
     */
    ProjectInputRaster(QDomElement *elProjectInput);

    void Init();
};

}

#endif // PROJECTINPUTRASTER_H
