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

    inline ~ProjectInputCSV(){}

    /**
     * @brief ProjectInputCSV
     * @param source
     */
    ProjectInputCSV(const ProjectInputCSV &source);

    /**
     * @brief operator =
     * @param source
     */
    void operator =(ProjectInputCSV &source);

    inline ProjectInput * Clone(){ return new ProjectInputCSV(*this); }

    /**
     * @brief getXField
     * @return
     */
    inline QString getXField() { return m_sXFieldName; }

    /**
     * @brief getYField
     * @return
     */
    inline QString getYField() { return m_sYFieldName; }

    /**
     * @brief getValueField
     * @return
     */
    inline QString getValueField() { return m_sFieldName; }

    /**
     * @brief CreateRasterFromCSV
     * @param sCSVFilePath
     */
    void CreateRasterFromCSV(QString sCSVFilePath);

    /**
     * @brief Prepare
     * @param csvRasterMeta
     */
    void Prepare(RasterManager::RasterMeta * TemplateRaster, QString NewPath);

    /**
     * @brief Init
     * @param sXFieldName
     * @param sYFieldName
     */
    void Init(QString sXFieldName, QString sYFieldName);
private:

    QString m_sXFieldName;
    QString m_sYFieldName;
    QString m_sFieldName;

};

}

#endif // PROJECTINPUTCSV_H
