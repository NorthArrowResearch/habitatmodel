#ifndef PROJECT_H
#define PROJECT_H

#include <QHash>
#include <QDomElement>
#include <QDir>

#include "modelcore_global.h"
#include "hmvariable.h"
#include "unit.h"
#include "hsc.h"
#include "projectinputcsv.h"
#include "projectinputraster.h"
#include "projectinputvector.h"
#include "projectinput.h"

#include "namedobject.h"

namespace HabitatModel{

enum ProjectInputTypeCodes {
    PROJECT_INPUT_UNDEFINED = 0,
    PROJECT_INPUT_RASTER = 1,
    PROJECT_INPUT_VECTOR = 2,
    PROJECT_INPUT_CSV = 3
};


class MODELCORESHARED_EXPORT Project : public NamedObjectWithID
{
public:

    /**
     * @brief Project
     * @param sXMLConfig
     * @param sXMLOutput
     * @param sXMLLogFile
     */
    Project(QString sXMLConfig, QString sXMLOutput, QString sXMLLogFile);

    ~Project();

    /**
     * @brief Load the xml file from a path string
     * @param sXMLConfig
     */
    void Load(QString sXMLConfig);

    void LoadHMVariables();
    void LoadLookupTable();
    void LoadUnits();
    void LoadHSCs();
    void LoadHSCCoordinatePairs();
    void LoadProjectInputs();

    /**
     * @brief GetConfig
     * @return
     */
    inline static QDomElement * GetConfig() { return &m_elConfig; }

    /**
     * @brief GetConfigPath
     * @return
     */
    inline static QDir * GetConfigPath() { return m_ConfigPath; }

    /**
     * @brief GetTmpPath
     * @return
     */
    inline static QDir * GetTmpPath() { return m_TmpPath; }

    /**
     * @brief GetLookupTableItem
     * @param nid
     * @return
     */
    inline static NamedObjectWithID * GetLookupTableItem(int nlistid) { return m_lookup_table.value(nlistid); }

    /**
     * @brief GetLookupTableItem -- convenience function to get a lookup table value from
     *        a dom element and a string containing the name of the id element
     *
     * @param elItem
     * @param sValIDName
     * @return
     */
    inline static NamedObjectWithID * GetLookupTableItem(QDomElement *elItem, QString sValIDName)
    {
        int nValID = elItem->firstChildElement(sValIDName).text().toInt();
        return GetLookupTableItem(nValID);
    }

    /**
     * @brief GetLookupTableItem
     * @param elItem
     * @param sValIDName
     * @return
     */
    inline static NamedObjectWithID * GetLookupTableItem(QDomNode *elItem, QString sValIDName)
    {
        int nValID = elItem->firstChildElement(sValIDName).text().toInt();
        return GetLookupTableItem(nValID);
    }

    /**
     * @brief GetVariable
     * @param nid
     * @return
     */
    inline static HMVariable * GetVariable(int nid){ return m_hmvariable_store.value(nid); }

    /**
     * @brief GetVariable
     * @param elItem
     * @param sValIDName
     * @return
     */
    inline static HMVariable * GetVariable(QDomElement *elItem, QString sValIDName){
        int nVarID = elItem->firstChildElement(sValIDName).text().toInt();
        return GetVariable(nVarID);
    }

    /**
     * @brief GetUnit
     * @param nid
     * @return
     */
    inline static Unit * GetUnit(int nid){ return m_unit_store.value(nid); }

    /**
     * @brief GetUnit
     * @param elItem
     * @param sUnitName
     * @return
     */
    inline static Unit * GetUnit(QDomElement *elItem, QString sUnitName){
        int nVarID = elItem->firstChildElement(sUnitName).text().toInt();
        return GetUnit(nVarID);
    }

    /**
     * @brief GetHSC
     * @param nid
     * @return
     */
    inline static HSC * GetHSC(int nid){ return m_HSC_store.value(nid); }

    /**
     * @brief GetHSC
     * @param elItem
     * @param sHSCName
     * @return
     */
    inline static HSC * GetHSC(QDomElement *elItem, QString sHSCName){
        int nVarID = elItem->firstChildElement(sHSCName).text().toInt();
        return GetHSC(nVarID);
    }

    /**
     * @brief Project::GetProjectInputIterator
     * @return
     */
    inline static QHashIterator<int, ProjectInput *> GetProjectInputIterator() {
        QHashIterator<int, ProjectInput *> i(m_project_inputs_store);
        return i;
    }

    /**
     * @brief GetProjectType
     * @param sInputFilePath QString to filepath
     * @return int enum ProjectInputTypeCodes
     */
    ProjectInputTypeCodes GetInputType(QString sInputFilePath);

    /**
     * @brief PrepareProjectInputs
     */
    void PrepareProjectInputs();

    /**
     * @brief GetRasterExtentMeta
     * @return
     */
    inline static RasterManager::RasterMeta * GetRasterExtentMeta(){
        return m_RasterTemplateMeta;
    }

private:
    // Project Attributes
    QString qd_date_created;
    QString s_created_by;

    QString s_computer;
    NamedObjectWithID * m_CHaMP_site;
    NamedObjectWithID * m_CHaMP_watershed;
    QString s_description;

    // The path to that config file
    static QDir * m_ConfigPath;
    static QDir * m_TmpPath;

    // The entire config file dom
    static QDomElement m_elConfig;

    // Stores of objects related to projects that we will
    // need to be able to lookup
    static QHash<int, HMVariable *> m_hmvariable_store;
    static QHash<int, Unit *> m_unit_store;

    static QHash<int, HSC *> m_HSC_store;

    static QHash<int, NamedObjectWithID *> m_lookup_table;
    static QHash<int, ProjectInput *> m_project_inputs_store;
    static RasterManager::RasterMeta * m_RasterTemplateMeta;

};

}

#endif // PROJECT_H
