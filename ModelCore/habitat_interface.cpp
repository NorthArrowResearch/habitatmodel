#define MY_DLL_EXPORT
#include "habitat_interface.h"
#include "habitatmodel_global.h"
#include "habitat_exception.h"
#include "project.h"

namespace HabitatModel{

extern "C" DLL_API int RunSimulations(const char * psProjectRoot,
                                      const char * psXMLInputDef,
                                      const char * psXMLInputConf,
                                      const char * psXMLOutput,
                                      char * sErr, unsigned int iBufferSize)
{
    int eResult = PROCESS_OK;
    try{
        HabitatModel::Project theProject(psProjectRoot, psXMLInputDef, psXMLInputConf, psXMLOutput);
        eResult = theProject.Run();
    }
    catch (HabitatException& e){
        const QByteArray qbErr = e.GetReturnMsgAsString().toLocal8Bit();
        const char * pHabErr = qbErr.data();
        strncpy(sErr, pHabErr, iBufferSize);
        sErr[ iBufferSize - 1 ] = 0;
        return e.GetErrorCode();
    }
    catch (std::exception& e){
        eResult = UNDEFINED_ERROR;
    }

    return eResult;
}

extern "C" DLL_API void GetReturnCodeAsString(unsigned int eErrorCode, char * sErr)
{
    const char * pHabErr = HabitatException::GetReturnCodeOnlyAsString(eErrorCode);
    strncpy(sErr, pHabErr, ERRBUFFERSIZE);
    sErr[ ERRBUFFERSIZE - 1 ] = 0;
}

}
