#ifndef PROJECTINPUT_H
#define PROJECTINPUT_H

#include <QDir>
#include <QString>
#include "namedobject.h"


namespace RasterManager{
    class RasterMeta;
}

namespace HabitatModel{

enum ProjectInputTypeCodes {
    PROJECT_INPUT_UNDEFINED = 0,
    PROJECT_INPUT_RASTER = 1,
    PROJECT_INPUT_VECTOR = 2,
    PROJECT_INPUT_CSV = 3
};
class RasterMeta;
class ProjectInputVector;
class ProjectInputRaster;
class ProjectInputCSV;
class Unit;
class HMVariable;

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
     * @brief Prepare
     * @param TemplateRaster
     * @param NewPath
     */
    virtual void Prepare(RasterManager::RasterMeta * TemplateRasterMeta) = 0;

    /**
     * @brief SetValueFieldName
     * @param sValueField
     */
    virtual void SetValueFieldName(QString sValueField ) = 0;

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
    void Init(QString sCreatedOn, QString sProjectPath, NamedObjectWithID *pSourceType, QString sUtilizationRasterFileName, QString sHSOutputFileName);


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
    inline QString GetSourceFilePath() const { return m_sourcefilepath; }

    /**
     * @brief getFileName
     * @return
     */
    inline QString GetInputFileName() const { return QFileInfo(m_sourcefilepath).fileName(); }

    /**
     * @brief getRasterInputFileName
     * @return
     */
    inline QString GetPreparedRasterFileName() const { return m_preparedrasterfilename; }

    /**
     * @brief SetRasterOutPutFileName
     * @param sOutputFile
     * @return
     */
    inline void SetUtilizationRasterFileName(QString sOutputFile) {  m_preparedrasterfilename = sOutputFile; }

    /**
     * @brief GetHSOutputRasterFileName
     * @return
     */
    inline QString GetHSOutputRasterFileName() const { return m_hsoutputfilepath; }

    /**
     * @brief SetHSOutputRasterFileName
     * @param sOutputFile
     */
    inline void SetHSOutputRasterFileName(QString sOutputFile) {  m_hsoutputfilepath = sOutputFile; }

private:

    NamedObjectWithID * m_source_type;

    QString m_screated_on;

    // Absolute path to source file
    QString m_sourcefilepath;
    QString m_hsoutputfilepath;
    QString m_preparedrasterfilename;


};


}
#endif // PROJECTINPUT_H
