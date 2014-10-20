#ifndef PROJECTINPUT_H
#define PROJECTINPUT_H

#include "QDate"

#include "unit.h"
#include "hmvariable.h"
#include "namedobject.h"
#include "project.h"

namespace HabitatModel{

class ProjectInput : public NamedObjectWithID
{
public:

    ProjectInput();
    Project getProject();
    NamedObjectWithID getSourceType();
    QDate getCreatedOn();
    QString getSourcePath();
    HMVariable getVariable();
    Unit getUnit();

private:

    Project * m_project;
    NamedObjectWithID * m_source_type;

    QDate qd_created_on;
    QString s_sourcepath;

    HMVariable * m_variable;
    Unit * m_unit;
};


}
#endif // PROJECTINPUT_H
