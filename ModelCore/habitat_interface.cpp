#define MY_DLL_EXPORT
#include "habitat_interface.h"
#include "habitatmodel_global.h"

namespace HabitatModel{

extern "C" DLL_API int RunSimulations(const char * psXMLInput,
                                      const char * psXMLOutput)
{
    return PROCESS_OK;
}


}
