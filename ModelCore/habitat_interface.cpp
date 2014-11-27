#define MY_DLL_EXPORT
#include "habitat_interface.h"
#include "habitatmodel_global.h"
#include "project.h"

namespace HabitatModel{

extern "C" DLL_API int RunSimulations(const char * psXMLInput,
                                      const char * psXMLOutput)
{
    int eResult = PROCESS_OK;
    HabitatModel::Project theProject(psXMLInput, psXMLOutput);
    eResult = theProject.Run();
    return eResult;
}


}
