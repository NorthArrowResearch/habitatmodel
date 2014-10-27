#ifndef PROJECTINPUTCSV_H
#define PROJECTINPUTCSV_H

#include <QString>
#include "projectinput.h"
#include "raster.h"


namespace HabitatModel{

class ProjectInputCSV : public ProjectInput
{
public:

    /**
     * @brief ProjectInputCSV
     * @param elProjectInput
     */
    ProjectInputCSV(QDomElement * elProjectInput);

    /**
     * @brief getXField
     * @return
     */
    QString getXField();

    /**
     * @brief getYField
     * @return
     */
    QString getYField();

    /**
     * @brief getValueField
     * @return
     */
    QString getValueField();

    /**
     * @brief CreateRasterFromCSV
     * @param sCSVFilePath
     */
    void CreateRasterFromCSV(QString sCSVFilePath);

    /**
     * @brief Prepare
     * @param csvRasterMeta
     */
    void Prepare(RasterManager::RasterMeta * csvRasterMeta);

private:

    QString m_sXFieldName;
    QString m_sYFieldName;
    QString m_sFieldName;

};

}

#endif // PROJECTINPUTCSV_H
