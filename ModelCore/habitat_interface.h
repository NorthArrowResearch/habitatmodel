#ifndef HABITAT_INTERFACE_H
#define HABITAT_INTERFACE_H

#include "habitatmodel_global.h"
#include "habitat_exception.h"

namespace HabitatModel{

const int ERRBUFFERSIZE = 1024;

/**
 * @brief RunSimulations
 * @param psXMLInput
 * @param psXMLOutput
 * @return
 */
extern "C" DLL_API int RunSimulations(const char * psProjectRoot,
                                      const char *psXMLInputDef, const char *psXMLInputConf,
                                      const char * psXMLOutput, char *sErr, unsigned int iBufferSize);


/**
 * @brief GetReturnCodeAsString
 * @param eErrorCode
 * @return
 */
extern "C" DLL_API void GetReturnCodeAsString(unsigned int eErrorCode, char * sErr, unsigned int iBufferSize);

}

#endif // HABITAT_INTERFACE_H
