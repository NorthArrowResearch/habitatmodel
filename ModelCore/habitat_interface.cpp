#define MY_DLL_EXPORT
#include "habitat_interface.h"
#include "habitatmodel_global.h"
#include "project.h"
#include "habitat_exception.h"

namespace HabitatModel{

extern "C" DLL_API int RunSimulations(const char * psProjectRoot,
                                      const char * psXMLInput,
                                      const char * psXMLOutput)
{
    int eResult = PROCESS_OK;
    try{
        HabitatModel::Project theProject(psProjectRoot, psXMLInput, psXMLOutput);
        eResult = theProject.Run();
    }
    catch (HabitatException& e){
        return e.GetErrorCode();
    }
    catch (std::exception& e){
        eResult = UNDEFINED_ERROR;
    }

    return eResult;
}

}
