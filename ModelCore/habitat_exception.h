#ifndef HABITAT_EXCEPTION_H
#define HABITAT_EXCEPTION_H
#include <QString>
#include "exception"

namespace HabitatModel{

enum ProjectErrorCodes {
    PROCESS_OK = 0,
    FILE_NOT_FOUND = 1,
    DIRECTORY_NOT_FOUND = 2,
    FILE_PRESENT = 3,
    DIRECTORY_PRESENT = 4,

    DEBUG = -1,
    UNDEFINED_ERROR = 999
};

class HabitatException :public std::exception
{
public:
    inline HabitatException(ProjectErrorCodes nErrorCode){
        m_nErrorCode = nErrorCode;
        m_sEvidence = "";
    }
    inline HabitatException(ProjectErrorCodes nErrorCode, QString sMsg){
        m_nErrorCode = nErrorCode;
        m_sEvidence = sMsg;
    }
    inline int GetErrorCode(){ return m_nErrorCode; }

    // Define the error messages
    inline static const char * GetReturnCodeOnlyAsString(int eErrorCode){
        switch (eErrorCode) {
        case PROCESS_OK:
            return "process completed successfully";
        case FILE_NOT_FOUND:
            return "File Not Found";
        case DIRECTORY_NOT_FOUND:
            return "Directory Not Found";
        case FILE_PRESENT:
            return "File Already Present";
        case DIRECTORY_PRESENT:
            return "Directory Already Present";
        case DEBUG:
            return "Input has flooped the output. Did you yurtify it before vorticating your quiznozzles?";
        default:
            return "Unhandled Raster Manager return code.";
        }
    };

    /**
     * @brief GetReturnMsgAsString: This function returns more information than GetReturnCodeOnlyAsString
     * @return
     */
    inline QString GetReturnMsgAsString(){
        QString sOutput = GetReturnCodeOnlyAsString(m_nErrorCode);
        if (m_sEvidence.length() > 0){
            sOutput = sOutput + ": " + m_sEvidence;
        }
        return sOutput;
    }

private:
    int m_nErrorCode;
    QString m_sEvidence;
};

}
#endif // HABITAT_EXCEPTION_H
