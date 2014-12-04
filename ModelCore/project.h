#ifndef PROJECT_H
#define PROJECT_H

#include <QHash>
#include <QDomElement>
#include <QDir>

#include "habitatmodel_global.h"
#include "namedobject.h"
#include "habitat_exception.h"
#include "projectinput.h"
#include "habitat_interface.h"
#include "xmlfile.h"

namespace HabitatModel{

class Simulation;
class HSC;
class Unit;
class HMVariable;

class DLL_API Project
{
public:

    /**
     * @brief Project
     * @param psXMLInput
     * @param psXMLOutput
     */
    Project(const char * psProjectRoot, const char *psXMLInput,
            const char * psXMLOutput);

    ~Project();

    /**
     * @brief Run: This is the main function that does all the work
     * @param psXMLInput
     * @param psXMLOutput
     */
    int Run();


    /**
     * @brief Load the xml file from a path string
     * @param sXMLConfig
     */
    void LoadHMVariables();
    void LoadLookupTable();
    void LoadUnits();
    void LoadHSCs();
    void LoadHSCCoordinatePairs();
    void LoadProjectDataSources();
    void LoadSimulations();

    /**
     * @brief LoadHSC
     * @param nNewHSCID
     * @param nType
     * @return
     */
    HSC * LoadHSC(int nNewHSCID, int nType);

    /**
     * @brief GetConfigDom
     * @return
     */
    inline static QDomDocument * GetConfigDom() { return m_elConfig; }

    /**
     * @brief GetOutputXML
     * @return
     */
    inline static XMLFile * GetOutputXML(){ return m_XMLOutput; }

    /**
     * @brief GetConfigPath
     * @return
     */
    inline static QDir * GetProjectRootPath() { return m_ProjectRootDir; }

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
    inline static HMVariable * GetHMVariable(int nid){ return m_hmvariable_store.value(nid); }

    /**
     * @brief GetProjectInput
     * @param nid
     * @return
     */
    inline static ProjectInput * GetProjectInput(int nid){ return m_raw_project_inputs_store.value(nid); }
    /**
     * @brief GetVariable
     * @param elItem
     * @param sValIDName
     * @return
     */
    inline static HMVariable * GetVariable(QDomElement *elItem, QString sValIDName){
        int nVarID = elItem->firstChildElement(sValIDName).text().toInt();
        return GetHMVariable(nVarID);
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
        QHashIterator<int, ProjectInput *> i(m_raw_project_inputs_store);
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
     * @brief GetRawProjectInputsStore
     * @return
     */
    inline static QHash<int, ProjectInput *> GetRawProjectInputsStore(){ return m_raw_project_inputs_store; }

    static void ProjectError(int nErrorCode);
    static void ProjectError(int nErrorCode, QString m_sEvidence);

    /**
     * @brief SanitizePath
     * @param sPath
     * @return
     */
    static QString SanitizePath(QString sPath);

    /**
     * @brief EnsureFile makes sure there's a directory waiting for the file and deletes any
     *        matching file to make room.
     * @param sFilePath
     */
    static void EnsureFile(QString sFilePath);

private:

    // Project Attributes
    QString qd_date_created;
    QString s_created_by;

    QString s_computer;
    NamedObjectWithID * m_CHaMP_site;
    NamedObjectWithID * m_CHaMP_watershed;
    QString s_description;

    // The path to that config file
    static QDir * m_ProjectRootDir;

    // Our input XML and OUTput log file
    static XMLFile * m_XMLInput;
    static XMLFile * m_XMLOutput;
    static QDomDocument * m_elConfig;

    // Stores of objects related to projects that we will
    // need to be able to lookup
    static QHash<int, HMVariable *> m_hmvariable_store;
    static QHash<int, Unit *> m_unit_store;

    static QHash<int, HSC *> m_HSC_store;

    static QHash<int, NamedObjectWithID *> m_lookup_table;
    static QHash<int, ProjectInput *> m_raw_project_inputs_store;
    static QHash<int, Simulation *> m_simulation_store;

};

}

#endif // PROJECT_H
