#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include "QDate"

#include "namedobject.h"

namespace HabitatModel{


class Project : public NamedObjectWithID
{
public:
    Project();

private:

    QDate qd_date_created;
    QString s_created_by;

    QString s_computer;
    NamedObjectWithID * m_CHaMP_site;
    NamedObjectWithID * m_CHaMP_watershed;
    QString s_description;
};

}

#endif // PROJECT_H
