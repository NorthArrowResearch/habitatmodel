#ifndef HABITATMODELENGINE_H
#define HABITATMODELENGINE_H

#include <QString>


class HabitatModelEngine
{
public:
    HabitatModelEngine(int argc, char * argv[]);

    QString GetFile(int argc, char *argv[], int nIndex, bool bMustExist);
private:
    void RunHabitatModel(int argc, char * argv[]);
};

#endif // HABITATMODELENGINE_H
