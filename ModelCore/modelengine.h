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

    static QDomElement *GetConfig();
    static HMVariable * GetVariable(int nid);
    static Unit * GetUnit(int nid);
    static NamedObjectWithID * GetLookupTableValue(int nid);

private:
    static QHash<int, HMVariable *> m_hmvariable_store;
    static QHash<int, Unit *> m_unit_store;
    static QHash<int, NamedObjectWithID *> m_lookup_table;
    static QDomElement m_elConfig;

};

}

#endif // MODELENGINE_H
