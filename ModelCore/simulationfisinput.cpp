#include "simulationfisinput.h"

namespace HabitatModel{

//<SimulationFISInputs>
//  <ProjectInputID>2</ProjectInputID>
//  <SimulationID>1</SimulationID>
//  <FISInputName>Depth</FISInputName>
//  <FISOutputPath>Simulations\SAMPLE_FIS\Outputs\FISOutputs\Water Depth.tif</FISOutputPath>
//  <FISPreparedPath>Simulations\SAMPLE_FIS\PreparedInputs\Water Depth.tif</FISPreparedPath>
//</SimulationFISInputs>

SimulationFISInput::SimulationFISInput(QDomElement elFISInput)
    : NamedObjectWithID("temp", 999)
{
    bool bProjectVariableFound = false;
    int nProjectInputID = elFISInput.firstChildElement("ProjectInputID").text().toInt();

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

            m_variable = Project::GetHMVariable( elProjectVariable.firstChildElement("VariableID").text().toInt() );

            int nDataSourceID = elProjectVariable.firstChildElement("DataSourceID").text().toInt();
            ProjectInput * pOriginalInput = Project::GetProjectInput(nDataSourceID);

            // Now we need a copy of the project input (both an object copy and a copy of the file)
            m_project_input = pOriginalInput->Clone();
            //             CSVs need a data field added to them
            if ( dynamic_cast <ProjectInputCSV *> ( m_project_input )){
                QString sValueField = elProjectVariable.firstChildElement("ValueField").text();
                m_project_input->SetValueFieldName(sValueField);
            }

            // The prepeared file path is explicitly given to us in the XML
            QString sFISPreparedPath = Project::SanitizePath(elFISInput.firstChildElement("FISPreparedPath").text());
            m_project_input->SetUtilizationRasterFileName(Project::GetProjectRootPath()->filePath(sFISPreparedPath));

            // The prepeared file path is explicitly given to us in the XML
            QString sFisInputName = elFISInput.firstChildElement("FISInputName").text();
            m_sFISInputName = Project::SanitizePath(sFisInputName);

            // The prepeared file path is explicitly given to us in the XML
//            QString sFISOutputPath = Project::SanitizePath(elFISInput.firstChildElement("FISOutputPath").text());
//            m_project_input->SetOutputRasterFileName(Project::GetProjectRootPath()->filePath(sFISOutputPath));

        }
    }

    // No project variable found for this FIS input. We must fail
    if (!bProjectVariableFound)
        Project::ProjectError(DOM_NODE_MISSING, QString("Missing <ProjectVariables> node for FIS input with ID:").arg(nProjectInputID) );

}

SimulationFISInput::~SimulationFISInput()
{
    delete m_project_input;
}

}
