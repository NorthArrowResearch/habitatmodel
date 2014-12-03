#ifndef PROJECTINPUTCSV_H
#define PROJECTINPUTCSV_H

#include <QString>
#include "projectinput.h"

namespace RasterManager{
    class RasterMeta;
}

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

    inline ProjectInput * Clone(){ return new ProjectInputCSV(*this); }

    /**
     * @brief getXField
     * @return
     */
    inline QString GetXFieldName() const { return m_sXFieldName; }

    /**
     * @brief getYField
     * @return
     */
    inline QString GetYFieldName() const { return m_sYFieldName; }

    /**
     * @brief getValueField
     * @return
     */
    inline QString GetValueFieldName() const { return m_sFieldName; }


    inline void SetValueFieldName(QString sValueField){ if (!sValueField.isEmpty()) m_sFieldName = sValueField; }
    /**
     * @brief CreateRasterFromCSV
     * @param sCSVFilePath
     */
    void CreateRasterFromCSV(QString sCSVFilePath);

    /**
     * @brief Prepare
     * @param csvRasterMeta
     */
    void Prepare(RasterManager::RasterMeta * TemplateRaster);

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
