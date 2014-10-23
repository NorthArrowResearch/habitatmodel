#ifndef PROJECT_H
#define PROJECT_H

#include <QHash>
#include <QDomElement>

#include "modelcore_global.h"
#include "hmvariable.h"
#include "unit.h"
#include "hsc.h"

#include "namedobject.h"

namespace HabitatModel{


class MODELCORESHARED_EXPORT Project : public NamedObjectWithID
{
public:
    Project(QString sXMLConfig, QString sXMLOutput, QString sXMLLogFile);
    void Load(QString sXMLConfig);

    void LoadHMVariables();
    void LoadLookupTable();
    void LoadUnits();
    void LoadHSCs();
    void LoadHSCCoordinatePairs();

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
    static NamedObjectWithID * GetLookupTableItem(QDomNode *elItem, QString sValIDName);

    static HMVariable * GetVariable(int nid);
    static HMVariable * GetVariable(QDomElement *elItem, QString sValIDName);

    static Unit * GetUnit(int nid);
    static Unit * GetUnit(QDomElement *elItem, QString sUnitName);

    static HSC * GetHSC(int nid);
    static HSC * GetHSC(QDomElement *elItem, QString sHSCName);

private:
    // Project Attributes
    QString qd_date_created;
    QString s_created_by;

    QString s_computer;
    NamedObjectWithID * m_CHaMP_site;
    NamedObjectWithID * m_CHaMP_watershed;
    QString s_description;

    // Stores of objects related to projects that we will
    // need to be able to lookup
    static QHash<int, HMVariable *> m_hmvariable_store;
    static QHash<int, Unit *> m_unit_store;

    static QHash<int, HSC *> m_HSC_store;

    static QHash<int, NamedObjectWithID *> m_lookup_table;
    static QDomElement m_elConfig;


};

}

#endif // PROJECT_H
