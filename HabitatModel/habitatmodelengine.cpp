#include "habitatmodelengine.h"

#include <QFile>
#include <QFileInfo>
#include <cstring>
#include <iostream>

#include "project.h"

namespace HabitatModelEngine {

HabitatModelEngine::HabitatModelEngine(int argc, char *argv[])
{
    bool bRecognizedCommand = false;

    if (argc == 4)
    {
        bRecognizedCommand = true;
        RunHabitatModel(argc, argv);

    }

    if (!bRecognizedCommand)
    {
        std::cout << "\n Habitat Model Command Line";
        std::cout << "\n    usage: habitatmodel <xml_config_file_path> <xml_output_file_path> <xml_log_file_path>";
        std::cout << "\n ";
        std::cout << "\n Arguments:";
        std::cout << "\n    xml_config_file_path: Absolute full path to existing xml config file.";
        std::cout << "\n    xml_output_file_path: Absolute full path to desired xml output file.";
        std::cout << "\n    xml_error_file_path: Absolute full path to desired xml log file.";

        std::cout << "\n";
        return;
    }

}

void HabitatModelEngine::RunHabitatModel(int argc, char *argv[])
{
    try
    {
        CheckFile(argc, argv, 1, true);  //sXMLConfig
        CheckFile(argc, argv, 2, false); //sXMLOutput
        CheckFile(argc, argv, 3, false); //sXMLLogFile

        HabitatModel::Project theSimulation(argv[1], argv[2], argv[3]);

        std::cout << "\nProcess completed successfully.\n";
    }
    catch (std::exception & ex)
    {
        std::cout <<"\nError: " << ex.what() << std::endl;
    }
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
                throw  std::runtime_error("The directory of the file you specified does not exist.");
            }

            sFile = sFile.trimmed();
            sFile = sFile.replace("\"","");
            if (bMustExist)
            {
                if (!QFile::exists(sFile))
                    throw  std::runtime_error("The specified input file does not exist.");
            }
            else
                if (QFile::exists(sFile))
                    throw std::runtime_error("The specified output file already exists.");
        }
    }
    else
        throw std::runtime_error("Insufficient command line arguments for operation.");

}


} //HabitatModelEngine
