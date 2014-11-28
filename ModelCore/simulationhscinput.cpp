#include "simulationhscinput.h"
#include "project.h"
#include "hsicurve.h"
#include "projectinput.h"
#include "projectinputcsv.h"
#include "projectinputraster.h"
#include "projectinputvector.h"

namespace HabitatModel{
/*
<ProjectVariables>
  <ProjectVariableID>3</ProjectVariableID>
  <DataSourceID>2</DataSourceID>
  <VariableID>21</VariableID>
  <UnitsID>8</UnitsID>
  <Title>DEM</Title>
</ProjectVariables>

<SimulationHSCInputs>
  <ProjectInputID>2</ProjectInputID>
  <HSICurveID>5</HSICurveID>
  <SimulationID>4</SimulationID>
</SimulationHSCInputs>

 Here's the chain of how this works
 -----------------------------------------------

    SimulationHSCInputs->ProjectInputID ----links-to--->  ProjectVariables->ProjectVariableID

    and then:

    ProjectVariables->DataSourceID ----links-to--->  ProjectDataSources->DataSourceID


*/
SimulationHSCInput::SimulationHSCInput(QDomElement elHSCInput, HSICurve * pHSICurve)
: NamedObjectWithID("temp", 999)
{
    bool bProjectVariableFound = false;

    int nProjectInputID = elHSCInput.firstChildElement("ProjectInputID").text().toInt();

    m_hsi_curve = pHSICurve;

    // Since ProjectVariables are sort of part of this model
    // We have to find the projectvariable that corresponds
    // to this hscinput.
    QDomNodeList elProjectVariables = Project::GetConfigDom()->elementsByTagName("ProjectVariables");

    for(int n= 0; n < elProjectVariables.length(); n++){
        QDomNode elProjectVariable = elProjectVariables.at(n);
        int nProjectVarID = elProjectVariable.firstChildElement("ProjectVariableID").text().toInt();

        if (nProjectInputID == nProjectVarID && !bProjectVariableFound){

            bProjectVariableFound = true;

            std::string sTitle = elProjectVariable.firstChildElement("Title").text().toStdString();
            // Set this object's name and ID.
            SetName(sTitle.c_str());
            SetID(nProjectVarID);

            m_units = Project::GetUnit( elProjectVariable.firstChildElement("UnitsID").text().toInt() );
            m_variable = Project::GetHMVariable(elProjectVariable.firstChildElement("VariableID").text().toInt() );

            int nDataSourceID = elProjectVariable.firstChildElement("DataSourceID").text().toInt();
            ProjectInput * pOriginalInput = Project::GetProjectInput(nDataSourceID);

            // Now we need a copy of the project input (both an object copy and a copy of the file)
            m_project_input = pOriginalInput->Clone();

        }
     }

    // No project variable found for this HSC input. We must fail
    if (!bProjectVariableFound)
        Project::ProjectError(DOM_NODE_MISSING, "Missing <ProjectVariables> node for HSC input with ID:" + nProjectInputID );


}

SimulationHSCInput::~SimulationHSCInput(){
    delete m_project_input;
    delete m_hsi_curve;
}


}

