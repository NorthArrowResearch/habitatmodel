#ifndef HABITAT_EXCEPTION_H
#define HABITAT_EXCEPTION_H

#include "exception"

namespace HabitatModel{

enum ProjectErrorCodes {
    PROCESS_OK = 0,
    DEBUG = -1,
    UNDEFINED_ERROR = 999
};

class HabitatException :public std::exception
{
public:
    HabitatException(ProjectErrorCodes nErrorCode);
    inline int GetErrorCode(){ return m_nErrorCode; }
    inline static const char * GetReturnCodeAsString(int eErrorCode){
        switch (eErrorCode)
        {
        case PROCESS_OK:
            return "process completed successfully.";
        case DEBUG:
            return "Input has flooped the output. Did you yurtify it before vorticating your quiznozzles?";
        default:
            return "Unhandled Raster Manager return code.";
        }
    };
private:
    int m_nErrorCode;
};

}
#endif // HABITAT_EXCEPTION_H
