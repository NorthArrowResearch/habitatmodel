#ifndef PROJECTINPUT_H
#define PROJECTINPUT_H

#include "unit.h"
#include "hmvariable.h"
#include "namedobject.h"
#include "rastermeta.h"
#include <QDir>

namespace HabitatModel{

class ProjectInputVector;
class ProjectInputRaster;
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
     * @brief getSourceType
     * @return
     */
    inline NamedObjectWithID * getSourceType() { return m_source_type;  }

    /**
     * @brief getCreatedOn
     * @return
     */
    inline QString getCreatedOn() { return m_screated_on; }

    /**
     * @brief getSourcePath
     * @return
     */
    inline QString getSourceFilePath() { return m_sourcefilepath; }

    /**
     * @brief getVariable
     * @return
     */
    inline HMVariable * getVariable() { return m_variable; }

    /**
     * @brief getUnit
     * @return
     */
    inline Unit * getUnit() { return m_unit; }

    /**
     * @brief getFileName
     * @return
     */
    inline QString getFileName(){
        return QFileInfo(m_sourcefilepath).fileName();
    }

    virtual void Prepare() = 0;


private:

    NamedObjectWithID * m_source_type;

    QString m_screated_on;
    QString m_sourcefilepath;

    HMVariable * m_variable;
    Unit * m_unit;


};


}
#endif // PROJECTINPUT_H
