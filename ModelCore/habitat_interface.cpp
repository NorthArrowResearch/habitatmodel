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


extern "C" DLL_API const char * HabitatModel::GetReturnCodeAsString(int eErrorCode)
{
    switch (eErrorCode)
    {
    case PROCESS_OK:
        return "process completed successfully.";

    case DEBUG:
        return "Input has flooped the output. Did you yurtify it before vorticating your quiznozzles?";

    default:
        return "Unhandled Raster Manager return code.";
    }

}

}
