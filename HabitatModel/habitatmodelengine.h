#ifndef HABITATMODELENGINE_H
#define HABITATMODELENGINE_H

#include <QString>

namespace HabitatModelEngine {

class HabitatModelEngine
{
public:
    HabitatModelEngine();
    int Run(int argc, char * argv[]);

    void CheckFile(int argc, char *argv[], int nIndex, bool bMustExist);

private:
    int RunHabitatModel(int argc, char * argv[]);
};

} // Habitat Model Engine

#endif // HABITATMODELENGINE_H
