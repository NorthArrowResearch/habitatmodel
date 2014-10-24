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
        QString sXMLConfig = GetFile(argc, argv, 1, true);
        QString sXMLOutput = GetFile(argc, argv, 2, false);
        QString sXMLLogFile = GetFile(argc, argv, 3, false);

        HabitatModel::Project theSimulation(sXMLConfig, sXMLOutput, sXMLLogFile);

        std::cout << "\nProcess completed successfully.\n";
    }
    catch (std::exception & ex)
    {
        std::cout <<"\nError: " << ex.what() << std::endl;
    }
}

QString HabitatModelEngine::GetFile(int argc, char * argv[], int nIndex, bool bMustExist)
{
    QString sFile;
    if (nIndex < argc)
    {
        // Enough arguments
        sFile = argv[nIndex];
        if (sFile.isNull() || sFile.isEmpty())
            throw "Command line missing a file path.";
        else
        {
            sFile = sFile.trimmed();
            sFile = sFile.replace("\"","");
            if (bMustExist)
            {
                if (!QFile::exists(sFile))
                    throw  "The specified input file does not exist.";
            }
            else
                if (QFile::exists(sFile))
                    throw std::runtime_error("The specified output file already exists.");
        }
    }
    else
        throw std::runtime_error("Insufficient command line arguments for operation.");

    return sFile;
}

} //HabitatModelEngine
