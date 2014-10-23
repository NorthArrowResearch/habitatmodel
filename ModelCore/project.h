#ifndef PROJECT_H
#define PROJECT_H

#include <QHash>
#include <QDomElement>

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

    static QDomElement *GetConfig();
    static NamedObjectWithID * GetLookupTableItem(int nid);
    /**
     * @brief GetLookupTableItem -- convenience function to get a lookup table value from
     *        a dom element and a string containing the name of the id element
     *
     * @param elItem
     * @param sValIDName
     * @return
     */
    static NamedObjectWithID * GetLookupTableItem(QDomElement *elItem, QString sValIDName);

    /**
     * @brief GetLookupTableItem
     * @param elItem
     * @param sValIDName
     * @return
     */
    static NamedObjectWithID * GetLookupTableItem(QDomNode *elItem, QString sValIDName);

    /**
     * @brief GetVariable
     * @param nid
     * @return
     */
    static HMVariable * GetVariable(int nid);

    /**
     * @brief GetVariable
     * @param elItem
     * @param sValIDName
     * @return
     */
    static HMVariable * GetVariable(QDomElement *elItem, QString sValIDName);

    /**
     * @brief GetUnit
     * @param nid
     * @return
     */
    static Unit * GetUnit(int nid);

    /**
     * @brief GetUnit
     * @param elItem
     * @param sUnitName
     * @return
     */
    static Unit * GetUnit(QDomElement *elItem, QString sUnitName);

    /**
     * @brief GetHSC
     * @param nid
     * @return
     */
    static HSC * GetHSC(int nid);

    /**
     * @brief GetHSC
     * @param elItem
     * @param sHSCName
     * @return
     */
    static HSC * GetHSC(QDomElement *elItem, QString sHSCName);

    /**
     * @brief GetProjectType
     * @param sInputFilePath QString to filepath
     * @return int enum ProjectInputTypeCodes
     */
    ProjectInputTypeCodes GetProjectType(QString sInputFilePath);


private:
    // Project Attributes
    QString qd_date_created;
    QString s_created_by;

    QString s_computer;
    NamedObjectWithID * m_CHaMP_site;
    NamedObjectWithID * m_CHaMP_watershed;
    QString s_description;

    // The entire config file dom
    static QDomElement m_elConfig;

    // Stores of objects related to projects that we will
    // need to be able to lookup
    static QHash<int, HMVariable *> m_hmvariable_store;
    static QHash<int, Unit *> m_unit_store;

    static QHash<int, HSC *> m_HSC_store;

    static QHash<int, NamedObjectWithID *> m_lookup_table;
    static QHash<int, ProjectInput *> m_project_inputs_store;

};

}

#endif // PROJECT_H
