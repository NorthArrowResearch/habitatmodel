#ifndef MODELENGINE_H
#define MODELENGINE_H

#include "modelcore_global.h"

namespace HabitatModel{

class MODELCORESHARED_EXPORT ModelEngine
{
public:
    ModelEngine(QString sXMLConfig, QString sXMLOutput, QString sXMLLogFile);
private:
    void Load();
};

}

#endif // MODELENGINE_H
