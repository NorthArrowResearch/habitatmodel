#ifndef PROJECTINPUT_H
#define PROJECTINPUT_H

#include "unit.h"
#include "hmvariable.h"
#include "namedobject.h"
#include "rastermeta.h"
#include <QDir>
#include <QString>

namespace HabitatModel{

enum ProjectInputTypeCodes {
    PROJECT_INPUT_UNDEFINED = 0,
    PROJECT_INPUT_RASTER = 1,
    PROJECT_INPUT_VECTOR = 2,
    PROJECT_INPUT_CSV = 3
};

class ProjectInputVector;
class ProjectInputRaster;
class ProjectInputCSV;

class ProjectInput : public NamedObjectWithID
{
public:

    /**
     * @brief Clone
     * @return
     */
    virtual ProjectInput * Clone()=0;

    /**
     * @brief ~ProjectInput
     */
    virtual inline ~ProjectInput(){ }

    /**
     * @brief ProjectInput
     * @param source
     */
    ProjectInput(const ProjectInput &source);

    /**
     * @brief Init
     * @param sCreatedOn
     * @param sProjectPath
     * @param pVariable
     * @param pSourceType
     * @param pUnit
     */
    void Init(QString sCreatedOn, QDir sProjectPath, HMVariable *pVariable, NamedObjectWithID *pSourceType, Unit *pUnit);


    /**
     * @brief ProjectInput
     * @param elProjectInput
     */
    ProjectInput(QDomElement *elProjectInput);

    /**
     * @brief getSourceType
     * @return
     */
    inline NamedObjectWithID * GetSourceType() const { return m_source_type;  }

    /**
     * @brief getCreatedOn
     * @return
     */
    inline QString GetCreatedOn() const { return m_screated_on; }

    /**
     * @brief getSourcePath
     * @return
     */
    inline QDir GetSourceFilePath() const { return m_sourcefilepath; }

    /**
     * @brief getVariable
     * @return
     */
    inline HMVariable * GetVariable() const { return m_variable; }

    /**
     * @brief getUnit
     * @return
     */
    inline Unit * GetUnit() const { return m_unit; }

    /**
     * @brief getFileName
     * @return
     */
    inline QString GetInputFileName() const { return QFileInfo(m_sourcefilepath.absolutePath()).fileName(); }
    /**
     * @brief getRasterFileName
     * @return
     */
    inline QFile * GetInputRasterFileName() const { return m_sRasterInputFile; }

    /**
     * @brief getRasterInputFileName
     * @return
     */
    inline QString getUtilizationRasterFileName() const { return m_sUtilizationFileName; }

    inline ProjectInputTypeCodes getInputType() const { return m_ProjectInputType; }

    /**
     * @brief SetRasterOutPutFileName
     * @param sOutputFile
     * @return
     */
    inline void setUtilizationRasterFileName(QString sOutputFile) {  m_sUtilizationFileName = sOutputFile; }

    virtual void Prepare(RasterManager::RasterMeta * TemplateRaster, QString NewPath) = 0;

protected:

    QFile * m_sRasterInputFile;
    ProjectInputTypeCodes m_ProjectInputType;


private:

    NamedObjectWithID * m_source_type;

    QString m_screated_on;
    // Absolute path to source file
    QDir m_sourcefilepath;

    QString m_sUtilizationFileName;

    HMVariable * m_variable;
    Unit * m_unit;

};


}
#endif // PROJECTINPUT_H
