#include "habitatmodelengine.h"
#include "rastermanager_interface.h"

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
        eResult = RunHabitatModel(argc, argv);
        return eResult;
    }
    else{
        std::cout << "\n Habitat Model Command Line";
        std::cout << "\n    usage: habitatmodel <xml_input_file_path> <xml_output_file_path>";
        std::cout << "\n ";
        std::cout << "\n Arguments:";
        std::cout << "\n    xml_config_file_path: Absolute full path to existing xml config file.";
        std::cout << "\n    xml_output_file_path: Absolute full path to desired xml output file.";

        std::cout << "\n";
        return eResult;
    }
}

int HabitatModelEngine::RunHabitatModel(int argc, char *argv[])
{
    int eResult = HabitatModel::PROCESS_OK;

    // Initialize GDAL drivers
    GDALAllRegister();

    HabitatModel::Project theProject(argv[1], argv[2], argv[3]);
    eResult = theProject.Run();

    // De-initialize all GDAL drivers.
    GDALDestroyDriverManager();

    return eResult;
}

} //HabitatModelEngine
