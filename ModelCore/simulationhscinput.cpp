#include "simulationhscinput.h"
#include "project.h"
#include "hsicurve.h"
#include "projectinput.h"
#include "projectinputcsv.h"
#include "projectinputraster.h"
#include "projectinputvector.h"
#include "habitat_exception.h"

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

            // Set this object's name and ID.
            SetName(elProjectVariable.firstChildElement("Title").text());
            SetID(nProjectVarID);

            m_units = Project::GetUnit( elProjectVariable.firstChildElement("UnitsID").text().toInt() );
            m_variable = Project::GetHMVariable( elProjectVariable.firstChildElement("VariableID").text().toInt() );

            int nDataSourceID = elProjectVariable.firstChildElement("DataSourceID").text().toInt();
            ProjectInput * pOriginalInput = Project::GetProjectInput(nDataSourceID);

            // Now we need a copy of the project input (both an object copy and a copy of the file)
            m_project_input = pOriginalInput->Clone();

            // CSVs need a data field added to them
            if ( dynamic_cast <ProjectInputCSV *> ( m_project_input )){
                QString sValueField = elProjectVariable.firstChildElement("ValueField").text();
                m_project_input->SetValueFieldName(sValueField);
            }

            // The prepeared file path is explicitly given to us in the XML
            QString sHSPreparedPath = Project::SanitizePath(elHSCInput.firstChildElement("HSPreparedPath").text());
            m_project_input->SetUtilizationRasterFileName(Project::GetProjectRootPath()->filePath(sHSPreparedPath));

            // The prepeared file path is explicitly given to us in the XML
            QString sHSOutputPath = Project::SanitizePath(elHSCInput.firstChildElement("HSOutputPath").text());
            m_project_input->SetOutputRasterFileName(Project::GetProjectRootPath()->filePath(sHSOutputPath));

        }
     }

    // No project variable found for this HSC input. We must fail
    if (!bProjectVariableFound)
        Project::ProjectError(DOM_NODE_MISSING, QString("Missing <ProjectVariables> node for HSC input with ID: %1").arg(nProjectInputID) );


}

SimulationHSCInput::~SimulationHSCInput(){
    delete m_project_input;
    delete m_hsi_curve;
}


}

