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
    void Prepare();



private:

    QString m_sXFieldName;
    QString m_sYFieldName;
    QString m_sFieldName;

};

}

#endif // PROJECTINPUTCSV_H
