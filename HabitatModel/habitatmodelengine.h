#ifndef HABITATMODELENGINE_H
#define HABITATMODELENGINE_H

#include <QString>

namespace HabitatModelEngine {

class HabitatModelEngine
{
public:
    HabitatModelEngine();

    /**
     * @brief Run
     * @param argc
     * @param argv
     * @return
     */
    int Run(int argc, char * argv[]);

    /**
     * @brief CheckFile
     * @param argc
     * @param argv
     * @param nIndex
     * @param bMustExist
     */
    void CheckFile(int argc, char *argv[], int nIndex, bool bMustExist);

    /**
     * @brief CheckFolder
     * @param argc
     * @param argv
     * @param nIndex
     * @param bMustExist
     */
    void CheckFolder(int argc, char *argv[], int nIndex, bool bMustExist);

private:
    int RunHabitatModel(int argc, char * argv[]);
};

} // Habitat Model Engine

#endif // HABITATMODELENGINE_H
