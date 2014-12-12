#ifndef HABITAT_INTERFACE_H
#define HABITAT_INTERFACE_H

#include "habitatmodel_global.h"
#include "habitat_exception.h"

namespace HabitatModel{

/**
 * @brief RunSimulations
 * @param psXMLInput
 * @param psXMLOutput
 * @return
 */
extern "C" DLL_API int RunSimulations(const char * psProjectRoot,
                                      const char *psXMLInput,
                                      const char * psXMLOutput);


/**
 * @brief GetReturnCodeAsString
 * @param eErrorCode
 * @return
 */
extern "C" DLL_API void GetReturnCodeAsString(int eErrorCode, const char **sErr);

}

#endif // HABITAT_INTERFACE_H
