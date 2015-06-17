#ifndef HABITAT_EXCEPTION_H
#define HABITAT_EXCEPTION_H
#include <QString>
#include "exception"

namespace HabitatModel{

enum ProjectErrorCodes {
    PROCESS_OK
    , FILE_NOT_FOUND
    , DIRECTORY_NOT_FOUND
    , FILE_PRESENT
    , DIRECTORY_PRESENT
    , FILE_WRITE_ERROR
    , FILE_READ_ONLY
    , DOM_LOAD_ERROR
    , DOM_NODE_MISSING
    , NO_RASTERS
    , MISSING_FIELD
    , RASTERMAN_EXCEPTION
    , XML_INPUT_ERROR
    , CSV_INPUT_ERROR

    , FIS_ERROR

    , DEBUG
    , UNDEFINED_ERROR
};

class HabitatException :public std::exception
{
public:
    inline HabitatException(int nErrorCode){ Init(nErrorCode, "");}
    inline HabitatException(int nErrorCode, QString sMsg){ Init(nErrorCode, sMsg); }
    inline ~HabitatException() throw(){}
    /**
     * @brief init
     */
    inline void Init(int nErrorCode, QString sMsg){
        m_nErrorCode = nErrorCode;
        m_sEvidence = sMsg;
    }
    /**
     * @brief GetErrorCode
     * @return
     */
    inline int GetErrorCode(){ return m_nErrorCode; }

    // Define the error messages
    inline static const char * GetReturnCodeOnlyAsString(int eErrorCode){
        switch (eErrorCode) {
        case PROCESS_OK:
            return "process completed successfully";
            break;
        case FILE_NOT_FOUND:
            return "File Not Found";
            break;
        case DIRECTORY_NOT_FOUND:
            return "Directory Not Found";
            break;
        case FILE_PRESENT:
            return "File Already Present";
            break;
        case DIRECTORY_PRESENT:
            return "Directory Already Present";
            break;
        case FILE_WRITE_ERROR:
            return "Failed to open file for writing";
            break;
        case FILE_READ_ONLY:
            return "File is Read Only";
            break;
        case DOM_NODE_MISSING:
            return "Missing Dom Node";
            break;
        case DEBUG:
            return "Input has flooped the output. Did you yurtify it before vorticating your quiznozzles?";
            break;
        case NO_RASTERS:
            return "No Rasters found. Exiting";
            break;
        case XML_INPUT_ERROR:
            return "Problem with the input XML";
            break;
        case RASTERMAN_EXCEPTION:
            return "RasterMan Exception";
            break;
        case CSV_INPUT_ERROR:
            return "Input CSV Error";
            break;
        case MISSING_FIELD:
            return "Field missing";
            break;
        case FIS_ERROR:
            return "FIS Error";
            break;
        default:
            return "Unhandled Raster Manager return code.";
            break;
        }
    };

    /**
     * @brief GetReturnMsgAsString: This function returns more information than GetReturnCodeOnlyAsString
     * @return
     */
    inline QString GetReturnMsgAsString(){
        QString sOutput = "";
        QString sErrMsg = GetReturnCodeOnlyAsString(m_nErrorCode);
        if (m_sEvidence.length() > 0){
            sOutput = sErrMsg + ": " + m_sEvidence;
        }
        return sOutput;
    }

private:
    int m_nErrorCode;
    QString m_sEvidence;
};

}
#endif // HABITAT_EXCEPTION_H
