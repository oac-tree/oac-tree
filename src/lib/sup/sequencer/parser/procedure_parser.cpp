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
 * Copyright (c) : 2010-2023 ITER Organization,
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
#include <sup/sequencer/generic_utils.h>
#include <sup/sequencer/sequence_parser.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anytype_helper.h>
#include <sup/dto/json_type_parser.h>

#include <fstream>
#include <sstream>

namespace sup
{
namespace sequencer
{
const std::string JSONTYPE_ATTRIBUTE_NAME = "jsontype";
const std::string JSONFILE_ATTRIBUTE_NAME = "jsonfile";

namespace
{
void ParsePreamble(Procedure* procedure, const sup::xml::TreeData& data,
                   const std::string& filename);
void ParseAndLoadPlugin(const sup::xml::TreeData& child);
void RegisterTypeInformation(Procedure* procedure, const sup::xml::TreeData& child,
                             const std::string& filename);
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
      "sup::sequencer::ParseProcedure(): root element of type <" + data.GetNodeName() +
      "> instead of: <" + Constants::PROCEDURE_ELEMENT_NAME + ">";
    throw ParseException(error_message);
  }
  auto result = std::unique_ptr<Procedure>(new Procedure());

  // Add current filename
  result->SetFilename(filename);

  // Load plugins and register types first
  ParsePreamble(result.get(), data, filename);

  // Add attributes
  result->AddAttributes(data.Attributes());

  // Parse child elements
  ParseProcedureChildren(result.get(), data);
  return result;
}

std::string GetFullPathName(const std::string& directory, const std::string& filename)
{
  if (filename.empty())
  {
    std::string error_message = "sup::sequencer::GetFullPathName(): empty filename passed";
    throw ParseException(error_message);
  }
  if (filename.front() == '/')
  {
    return filename;
  }
  return directory + filename;
}

std::string GetFileDirectory(const std::string& filename)
{
  auto pos = filename.find_last_of('/');
  if (pos == std::string::npos)
  {
    return {};
  }
  return filename.substr(0, pos + 1);
}

namespace
{
void ParsePreamble(Procedure* procedure, const sup::xml::TreeData& data,
                   const std::string& filename)
{
  for (const auto& child : data.Children())
  {
    if (child.GetNodeName() == Constants::PLUGIN_ELEMENT_NAME)
    {
      ParseAndLoadPlugin(child);
    }
    else if (child.GetNodeName() == Constants::REGISTERTYPE_ELEMENT_NAME)
    {
      RegisterTypeInformation(procedure, child, filename);
    }
  }
}

void ParseAndLoadPlugin(const sup::xml::TreeData& child)
{
  auto plugin_name = child.GetContent();
  if (plugin_name.empty())
  {
    std::string error_message =
      "sup::sequencer::ParseAndLoadPlugin(): mandatory content missing for element of type: <" +
      Constants::PLUGIN_ELEMENT_NAME + ">";
    throw ParseException(error_message);
  }
  LoadPlugin(plugin_name);
}

void RegisterTypeInformation(Procedure* procedure, const sup::xml::TreeData& child,
                             const std::string& filename)
{
  sup::dto::AnyType parsed_type;
  sup::dto::JSONAnyTypeParser parser;
  if (child.HasAttribute(JSONTYPE_ATTRIBUTE_NAME) == child.HasAttribute(JSONFILE_ATTRIBUTE_NAME))
  {
    std::string error_message =
      "sup::sequencer::RegisterTypeInformation(): element should contain exactly one attribute out "
      "of (" + JSONTYPE_ATTRIBUTE_NAME + ", " + JSONFILE_ATTRIBUTE_NAME + ")";
    throw ParseException(error_message);
  }
  if (child.HasAttribute(JSONTYPE_ATTRIBUTE_NAME))
  {
    if (!parser.ParseString(child.GetAttribute(JSONTYPE_ATTRIBUTE_NAME),
                            procedure->GetTypeRegistry()))
    {
      std::string error_message =
        "sup::sequencer::RegisterTypeInformation(): could not parse type: [" +
        child.GetAttribute(JSONTYPE_ATTRIBUTE_NAME) + "]";
      throw ParseException(error_message);
    }
    parsed_type = parser.MoveAnyType();
  }
  if (child.HasAttribute(JSONFILE_ATTRIBUTE_NAME))
  {
    auto json_filename = GetFullPathName(GetFileDirectory(filename),
                                         child.GetAttribute(JSONFILE_ATTRIBUTE_NAME));
    if (!parser.ParseFile(json_filename, procedure->GetTypeRegistry()))
    {
      std::string error_message =
        "sup::sequencer::RegisterTypeInformation(): could not parse file: [" + json_filename + "]";
      throw ParseException(error_message);
    }
    parsed_type = parser.MoveAnyType();
  }
  if (!procedure->RegisterType(parsed_type))
  {
    std::string error_message =
      "sup::sequencer::RegisterTypeInformation(): type could not be added to the registry: [" +
      sup::dto::AnyTypeToJSONString(parsed_type) + "]";
    throw ParseException(error_message);
  }
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
    auto name = var_data.HasAttribute(attributes::kNameAttribute)
                ? var_data.GetAttribute(attributes::kNameAttribute) : "";
    if (name.empty())
    {
      std::string error_message =
        "sup::sequencer::AddWorkspaceVariables(): variable with type <" + var_data.GetNodeName() +
        "> has no or an empty name";
      throw ParseException(error_message);
    }
    auto var = ParseVariable(var_data);
    if (!procedure->AddVariable(name, std::move(var)))
    {
      std::string error_message =
        "sup::sequencer::AddWorkspaceVariables(): adding variable with type [" +
        var_data.GetNodeName() + "] and name [" + name + "] to procedure failed";
      throw ParseException(error_message);
    }
  }
}

void ParseAndAddInstruction(Procedure *procedure, const sup::xml::TreeData &instr_data)
{
  auto instr = ParseInstruction(instr_data, procedure->GetFilename());
  procedure->PushInstruction(std::move(instr));
}

}  // unnamed namespace

}  // namespace sequencer

}  // namespace sup
