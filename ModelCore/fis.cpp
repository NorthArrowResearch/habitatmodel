#include "fis.h"
#include <QFile>


namespace HabitatModel{


//<FIS>
//  <FISID>555</FISID>
//  <FISRuleFile>Simulations\SAMPLE_FIS\Outputs\SAMPLE_FIS.tif</FISRuleFile>
//</FIS>

FIS::FIS(QDomElement * elFIS) : NamedObjectWithID(elFIS, "FISName", "FISID")
{
    QString sFISRulePath = Project::SanitizePath(elFIS->firstChildElement("FISRuleFile").text());
    m_sFISRuleFile = Project::GetProjectRootPath()->filePath(sFISRulePath);

    if (!QFile(m_sFISRuleFile).exists())
        Project::ProjectError(FILE_NOT_FOUND, QString("Could not find the FIS Rule File: %1").arg(m_sFISRuleFile));

}

FIS::~FIS()
{

}


}
