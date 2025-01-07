/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "procedure_parser.h"

#include "instruction_parser.h"
#include "variable_parser.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/sequence_parser.h>

#include <fstream>
#include <sstream>

namespace sup
{
namespace sequencer
{

namespace
{
void ParsePreamble(Procedure* procedure, const sup::xml::TreeData& data);
void RegisterTypeInformation(Procedure* procedure, const sup::xml::TreeData& child);
void ParseProcedureChildren(Procedure* procedure, const sup::xml::TreeData& data);
void AddWorkspaceVariables(Procedure* procedure, const sup::xml::TreeData& ws_data);
void ParseAndAddInstruction(Procedure* procedure, const sup::xml::TreeData& instr_data);
}  // unnamed namespace

std::unique_ptr<Procedure> ParseProcedure(const sup::xml::TreeData& data,
                                          const std::string& filename)
{
  if (data.GetNodeName() != Constants::PROCEDURE_ELEMENT_NAME)
  {
    std::string error_message =
      "ParseProcedure(): root element of type <" + data.GetNodeName() +
      "> instead of: <" + Constants::PROCEDURE_ELEMENT_NAME + ">";
    throw ParseException(error_message);
  }
  auto result = std::unique_ptr<Procedure>(new Procedure(filename));

  // Load plugins and register types first
  ParsePreamble(result.get(), data);

  // Add attributes
  result->AddAttributes(data.Attributes());

  // Parse child elements
  ParseProcedureChildren(result.get(), data);
  return result;
}

namespace
{
void ParsePreamble(Procedure* procedure, const sup::xml::TreeData& data)
{
  for (const auto& child : data.Children())
  {
    if (child.GetNodeName() == Constants::PLUGIN_ELEMENT_NAME)
    {
      auto plugin_name = child.GetContent();
      LoadPlugin(plugin_name);
      procedure->GetPreamble().AddPluginPath(plugin_name);
    }
    else if (child.GetNodeName() == Constants::REGISTERTYPE_ELEMENT_NAME)
    {
      RegisterTypeInformation(procedure, child);
    }
  }
}

void RegisterTypeInformation(Procedure* procedure, const sup::xml::TreeData& child)
{
  TypeRegistrationInfo::RegistrationMode registration_mode = TypeRegistrationInfo::kJSONFile;
  std::string registration_string;
  if (child.HasAttribute(Constants::REGISTERTYPE_JSON_TYPE_ATTRIBUTE)
      == child.HasAttribute(Constants::REGISTERTYPE_JSON_FILE_ATTRIBUTE))
  {
    std::string error_message =
        "RegisterTypeInformation(): element should contain exactly one attribute "
        "out of ("
        + Constants::REGISTERTYPE_JSON_TYPE_ATTRIBUTE + ", "
        + Constants::REGISTERTYPE_JSON_FILE_ATTRIBUTE + ")";
    throw ParseException(error_message);
  }
  if (child.HasAttribute(Constants::REGISTERTYPE_JSON_TYPE_ATTRIBUTE))
  {
    registration_mode = TypeRegistrationInfo::kJSONString;
    registration_string = child.GetAttribute(Constants::REGISTERTYPE_JSON_TYPE_ATTRIBUTE);
  }
  if (child.HasAttribute(Constants::REGISTERTYPE_JSON_FILE_ATTRIBUTE))
  {
    registration_mode = TypeRegistrationInfo::kJSONFile;
    registration_string = child.GetAttribute(Constants::REGISTERTYPE_JSON_FILE_ATTRIBUTE);
  }
  procedure->GetPreamble().AddTypeRegistration({registration_mode, registration_string});
}

void ParseProcedureChildren(Procedure* procedure, const sup::xml::TreeData& data)
{
  for (auto it = data.Children().begin(); it != data.Children().end(); ++it)
  {
    auto element_type = it->GetNodeName();
    if (element_type == Constants::WORKSPACE_ELEMENT_NAME)
    {
      AddWorkspaceVariables(procedure, *it);
    }
    else if (element_type == Constants::PLUGIN_ELEMENT_NAME
          || element_type == Constants::REGISTERTYPE_ELEMENT_NAME)
    {
      continue;  // Plugins were already handled.
    }
    else
    {
      // Every non workspace element of the Procedure node should be an instruction node
      ParseAndAddInstruction(procedure, *it);
    }
  }
}

void AddWorkspaceVariables(Procedure* procedure, const sup::xml::TreeData& ws_data)
{
  for (auto &var_data : ws_data.Children())
  {
    auto name = var_data.HasAttribute(Constants::NAME_ATTRIBUTE_NAME)
                ? var_data.GetAttribute(Constants::NAME_ATTRIBUTE_NAME) : "";
    if (name.empty())
    {
      std::string error_message =
        "AddWorkspaceVariables(): variable with type <" + var_data.GetNodeName() +
        "> has no or an empty name";
      throw ParseException(error_message);
    }
    auto var = ParseVariable(var_data);
    if (!procedure->AddVariable(name, std::move(var)))
    {
      std::string error_message =
        "AddWorkspaceVariables(): adding variable with type [" +
        var_data.GetNodeName() + "] and name [" + name + "] to procedure failed";
      throw ParseException(error_message);
    }
  }
}

void ParseAndAddInstruction(Procedure *procedure, const sup::xml::TreeData &instr_data)
{
  auto instr = ParseInstruction(instr_data);
  procedure->PushInstruction(std::move(instr));
}

}  // unnamed namespace

}  // namespace sequencer

}  // namespace sup
