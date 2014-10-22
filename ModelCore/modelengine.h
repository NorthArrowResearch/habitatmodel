#ifndef MODELENGINE_H
#define MODELENGINE_H

#include <QHash>
#include <QDomElement>

#include "modelcore_global.h"
#include "hmvariable.h"
#include "unit.h"

namespace HabitatModel{

class MODELCORESHARED_EXPORT ModelEngine
{
public:
    ModelEngine(QString sXMLConfig, QString sXMLOutput, QString sXMLLogFile);
    void Load(QString sXMLConfig);

    void LoadHMVariables();
    void LoadLookupTable();
    void LoadUnits();

    static QDomElement *GetConfig();
    static HMVariable * GetVariable(int nid);
    static Unit * GetUnit(int nid);
    static NamedObjectWithID * GetLookupTableItem(int nid);
    /**
     * @brief GetLookupTableItem -- convenience function to get a lookup table value from
     *        a dom element and a string containing the name of the id element
     *
     * @param elItem
     * @param sValIDName
     * @return
     */
    static NamedObjectWithID * GetLookupTableItem(QDomElement * elItem, QString sValIDName);

private:
    static QHash<int, HMVariable *> m_hmvariable_store;
    static QHash<int, Unit *> m_unit_store;

    static QHash<int, NamedObjectWithID *> m_lookup_table;
    static QDomElement m_elConfig;

};

}

#endif // MODELENGINE_H
