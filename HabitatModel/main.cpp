#include <iostream>
#include <stdio.h>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>

#include "habitatmodelengine.h"
#include "project.h"
#include "habitat_exception.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try
    {
        HabitatModelEngine::HabitatModelEngine * habmodel = new HabitatModelEngine::HabitatModelEngine();
        int eResult = habmodel->Run(argc, argv);
        if (eResult <= 0) {
            std::cout << std::endl << HabitatModel::GetReturnCodeAsString(eResult) << std::endl;
        }
        else {
            std::cerr << std::endl <<  HabitatModel::GetReturnCodeAsString(eResult) << std::endl;
            exit (EXIT_FAILURE);
        }

        exit (EXIT_SUCCESS);
    }
    catch (HabitatModel::HabitatException & e)
    {
        std::cerr <<"Error: " << e.GetReturnMsgAsString().toStdString() << std::endl;
        exit (EXIT_FAILURE);
    }
    catch (std::exception& e)
    {
        std::cerr <<"Error: " << e.what() << std::endl;
        exit (EXIT_FAILURE);
    }

    qApp->quit();

}
