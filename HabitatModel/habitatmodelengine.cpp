#include "habitatmodelengine.h"

#include <QFile>
#include <QFileInfo>
#include <cstring>
#include <iostream>

#include "project.h"

namespace HabitatModelEngine {

HabitatModelEngine::HabitatModelEngine(){ }

int HabitatModelEngine::Run(int argc, char *argv[])
{
    int eResult = HabitatModel::PROCESS_OK;
    if (argc == 4)
    {
        RunHabitatModel(argc, argv);
        return eResult;
    }
    else{
        std::cout << "\n Habitat Model Command Line";
        std::cout << "\n    usage: habitatmodel <xml_config_file_path>";
        std::cout << "\n ";
        std::cout << "\n Arguments:";
        std::cout << "\n    xml_config_file_path: Absolute full path to existing xml config file.";
        std::cout << "\n    xml_output_file_path: Absolute full path to desired xml output file.";
        std::cout << "\n    xml_error_file_path: Absolute full path to desired xml log file.";

        std::cout << "\n";
        return eResult;
    }
}

int HabitatModelEngine::RunHabitatModel(int argc, char *argv[])
{
    int eResult = HabitatModel::PROCESS_OK;

    CheckFile(argc, argv, 1, true);  //sXMLConfig

    HabitatModel::Project theSimulation(argv[1]);

    return eResult;
}

void HabitatModelEngine::CheckFile(int argc, char * argv[], int nIndex, bool bMustExist)
{

    if (nIndex < argc)
    {
        QString sFile = argv[nIndex];

        // Enough arguments
        if (sFile.isNull() || sFile.isEmpty())
            throw std::runtime_error("Command line missing a file path.");
        else
        {
            // Check if the directory the file exists in is actually there
            QDir sFilePath = QFileInfo(sFile).absoluteDir();
            if (!sFilePath.exists()){
                QString sErr = "The directory of the file you specified does not exist: " + sFilePath.absolutePath();
                throw  std::runtime_error(sErr.toStdString());
            }

            sFile = sFile.trimmed();
            sFile = sFile.replace("\"","");
            if (bMustExist)
            {
                if (!QFile::exists(sFile)){
                    QString sErr = "The specified input file does not exist: " + sFile;
                    throw  std::runtime_error(sErr.toStdString());
                }
            }
            else
                if (QFile::exists(sFile)){
                    QString sErr = "The specified output file already exists." + sFile;
                    throw  std::runtime_error(sErr.toStdString());
                }
        }
    }
    else
        throw std::runtime_error("Insufficient command line arguments for operation.");

}


} //HabitatModelEngine
