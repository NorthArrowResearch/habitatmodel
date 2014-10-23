#ifndef PROJECTINPUTCSV_H
#define PROJECTINPUTCSV_H

#include <QString>
#include "projectinput.h"

namespace HabitatModel{

class ProjectInputCSV : public ProjectInput
{
public:

    /**
     * @brief ProjectInputCSV
     * @param elProjectInput
     */
    ProjectInputCSV(QDomElement *elProjectInput);

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
private:

    QString m_sXField;
    QString m_sYField;
    QString m_FieldName;

};

}

#endif // PROJECTINPUTCSV_H
