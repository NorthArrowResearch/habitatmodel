#include "habitatmodelengine.h"
#include "rastermanager_interface.h"

#include <QFile>
#include <QFileInfo>
#include <cstring>
#include <iostream>
#include "habitat_exception.h"
#include "project.h"

namespace HabitatModelEngine {

HabitatModelEngine::HabitatModelEngine(){ }

int HabitatModelEngine::Run(int argc, char *argv[])
{
    int eResult = HabitatModel::PROCESS_OK;
    if (argc == 5)
    {
        eResult = RunHabitatModel(argc, argv);
        return eResult;
    }
    else{
        std::cout << "\n Habitat Model Command Line v"<<VERSION;
        std::cout << "\n    usage: habitatmodel <root_project_path> <xml_input_def_file_path> <xml_input_conf_file_path> <xml_output_file_path>";
        std::cout << "\n ";
        std::cout << "\n Arguments:";
        std::cout << "\n           root_project_path: Absolute full path to existing project folder.";
        std::cout << "\n     xml_input_def_file_path: Absolute full path to existing xml definitions file.";
        std::cout << "\n    xml_input_conf_file_path: Absolute full path to existing xml configuration file.";
        std::cout << "\n        xml_output_file_path: Absolute full path to desired xml output file.";

        std::cout << "\n";
        return eResult;
    }
}

int HabitatModelEngine::RunHabitatModel(int argc, char *argv[])
{
    int eResult = HabitatModel::PROCESS_OK;
    if (argc > 0){

        // Initialize GDAL drivers
        GDALAllRegister();

        HabitatModel::Project theProject(argv[1], argv[2], argv[3], argv[4]);
        eResult = theProject.Run();

        // De-initialize all GDAL drivers.
        GDALDestroyDriverManager();
    }

    return eResult;
}

} //HabitatModelEngine
