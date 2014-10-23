#ifndef PROJECTINPUT_H
#define PROJECTINPUT_H

#include "unit.h"
#include "hmvariable.h"
#include "namedobject.h"

namespace HabitatModel{

class ProjectInputVector;
class ProjectInputRaser;
class ProjectInputCSV;

class ProjectInput : public NamedObjectWithID
{
public:

    /**
     * @brief ProjectInput
     * @param sName
     * @param nID
     */
    ProjectInput(const char *sName, int nID);

    /**
     * @brief ProjectInput
     * @param elProjectInput
     */
    ProjectInput(QDomElement *elProjectInput);

    /**
     * @brief getProject
     * @return
     */
    //Project * getProject();

    /**
     * @brief getSourceType
     * @return
     */
    NamedObjectWithID * getSourceType();

    /**
     * @brief getCreatedOn
     * @return
     */
    QString getCreatedOn();

    /**
     * @brief getSourcePath
     * @return
     */
    QString getSourceFilePath();

    /**
     * @brief getVariable
     * @return
     */
    HMVariable * getVariable();

    /**
     * @brief getUnit
     * @return
     */
    Unit * getUnit();



private:

    NamedObjectWithID * m_source_type;

    QString m_screated_on;
    QString m_sourcefilepath;

    HMVariable * m_variable;
    Unit * m_unit;
};


}
#endif // PROJECTINPUT_H
