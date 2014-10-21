#include <iostream>
#include <stdio.h>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>

#include "habitatmodelengine.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try
    {
        HabitatModelEngine::HabitatModelEngine habitatmodel(argc, argv);
    }
    catch (std::exception& e)
    {
        std::cout << e.what();
    }

    qApp->quit();

}
