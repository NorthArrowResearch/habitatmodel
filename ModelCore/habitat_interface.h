#ifndef HABITAT_INTERFACE_H
#define HABITAT_INTERFACE_H

#include "habitatmodel_global.h"

namespace HabitatModel{

enum ProjectInputTypeCodes {
    PROJECT_INPUT_UNDEFINED = 0,
    PROJECT_INPUT_RASTER = 1,
    PROJECT_INPUT_VECTOR = 2,
    PROJECT_INPUT_CSV = 3
};

enum ProjectErrorCodes {
    PROCESS_OK = 0,
    DEBUG = -1
};

enum HSCTypes {
    HSC_CATEGORICAL = 0,
    HSC_INFLECTION = 1
};

/**
 * @brief RunSimulations
 * @param psXMLInput
 * @param psXMLOutput
 * @return
 */
extern "C" DLL_API int RunSimulations(const char * psXMLInput,
                                      const char * psXMLOutput);

}

#endif // HABITAT_INTERFACE_H
