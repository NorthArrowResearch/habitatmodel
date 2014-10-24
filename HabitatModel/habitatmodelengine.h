#ifndef HABITATMODELENGINE_H
#define HABITATMODELENGINE_H

#include <QString>

namespace HabitatModelEngine {

class HabitatModelEngine
{
public:
    HabitatModelEngine(int argc, char * argv[]);

    QString GetFile(int argc, char *argv[], int nIndex, bool bMustExist);
private:
    void RunHabitatModel(int argc, char * argv[]);
};

} // Habitat Model Engine

#endif // HABITATMODELENGINE_H
