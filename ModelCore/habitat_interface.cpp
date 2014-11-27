#define MY_DLL_EXPORT
#include "habitat_interface.h"
#include "habitatmodel_global.h"
#include "project.h"

namespace HabitatModel{

extern "C" DLL_API int RunSimulations(const char * psXMLInput,
                                      const char * psXMLOutput)
{
    HabitatModel::Project theSimulation(psXMLInput, psXMLOutput);
    return PROCESS_OK;
}


}
