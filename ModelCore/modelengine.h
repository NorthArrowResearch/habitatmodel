#ifndef MODELENGINE_H
#define MODELENGINE_H

#include <QHash>

#include "modelcore_global.h"
#include "hmvariable.h"
#include "unit.h"

namespace HabitatModel{

class MODELCORESHARED_EXPORT ModelEngine
{
public:
    ModelEngine(QString sXMLConfig, QString sXMLOutput, QString sXMLLogFile);
    HMVariable * GetVariable(int nid);
    Unit * GetUnit(int nid);
private:
    void Load(QString sXMLConfig);

    QHash<int, HMVariable *> m_hmvariable_store;
    QHash<int, Unit *> m_unit_store;

};

}

#endif // MODELENGINE_H
