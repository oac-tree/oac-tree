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
 * Copyright (c) : 2010-2022 ITER Organization,
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
#include <sup/sequencer/log.h>
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
static const std::string JSONTYPE_ATTRIBUTE_NAME = "jsontype";
static const std::string JSONFILE_ATTRIBUTE_NAME = "jsonfile";

namespace
{
void ParsePreamble(Procedure *procedure, const TreeData &data, const std::string &filename);
void ParseAndLoadPlugin(const TreeData &child);
void RegisterTypeInformation(Procedure *procedure, const TreeData &child,
                             const std::string &filename);
bool ParseProcedureChildren(Procedure *procedure, const TreeData &data);
bool AddWorkspaceVariables(Procedure *procedure, const TreeData &ws_data);
bool ParseAndAddInstruction(Procedure *procedure, const TreeData &instr_data);
}  // unnamed namespace

std::unique_ptr<Procedure> ParseProcedure(const TreeData &data, const std::string &filename)
{
  if (data.GetType() != Constants::PROCEDURE_ELEMENT_NAME)
  {
    std::string error_message =
      "sup::sequencer::ParseProcedure(): root element of type <" + data.GetType() +
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
  if (ParseProcedureChildren(result.get(), data))
  {
    return result;
  }
  return {};
}

std::string GetFullPathName(const std::string &directory, const std::string &filename)
{
  if (filename.empty())
  {
    log::Warning("sup::sequencer::GetFullPathName() - empty filename as argument");
    return {};
  }
  if (filename.front() == '/')
  {
    return filename;
  }
  return directory + filename;
}

std::string GetFileDirectory(const std::string &filename)
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
void ParsePreamble(Procedure* procedure, const TreeData& data, const std::string& filename)
{
  for (const auto &child : data.Children())
  {
    if (child.GetType() == Constants::PLUGIN_ELEMENT_NAME)
    {
      ParseAndLoadPlugin(child);
    }
    else if (child.GetType() == Constants::REGISTERTYPE_ELEMENT_NAME)
    {
      RegisterTypeInformation(procedure, child, filename);
    }
  }
}

void ParseAndLoadPlugin(const TreeData &child)
{
  auto plugin_name = child.GetContent();
  if (plugin_name.empty())
  {
    std::string error_message =
      "sup::sequencer::ParseAndLoadPlugin(): mandatory content missing for element of type: <" +
      Constants::PLUGIN_ELEMENT_NAME + ">";
    throw ParseException(error_message);
  }
  if (!LoadPlugin(plugin_name))
  {
    std::string error_message =
      "sup::sequencer::ParseAndLoadPlugin(): could not load plugin with name: [" + plugin_name + "]";
    throw ParseException(error_message);
  }
}

void RegisterTypeInformation(Procedure *procedure, const TreeData &child,
                             const std::string &filename)
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

bool ParseProcedureChildren(Procedure *procedure, const TreeData &data)
{
  bool status = true;
  for (auto it = data.Children().begin(); status && it != data.Children().end(); ++it)
  {
    auto element_type = it->GetType();
    if (element_type == Constants::WORKSPACE_ELEMENT_NAME)
    {
      status = AddWorkspaceVariables(procedure, *it);
    }
    else if (element_type == Constants::PLUGIN_ELEMENT_NAME || element_type == Constants::REGISTERTYPE_ELEMENT_NAME)
    {
      continue;  // Plugins were already handled.
    }
    else
    {
      // Every non workspace element of the Procedure node should be an instruction node
      status = ParseAndAddInstruction(procedure, *it);
    }
  }
  return status;
}

bool AddWorkspaceVariables(Procedure *procedure, const TreeData &ws_data)
{
  bool result = true;
  for (auto &var_data : ws_data.Children())
  {
    auto name = var_data.GetName();
    if (name.empty())
    {
      std::string error_message =
        "sup::sequencer::AddWorkspaceVariables(): variable with type <" + var_data.GetType() +
        "> has no or an empty name";
      throw ParseException(error_message);
    }
    auto var = ParseVariable(var_data);
    if (!var)
    {
      return false;
    }
    result = procedure->AddVariable(name, var.release()) && result;
  }
  return result;
}

bool ParseAndAddInstruction(Procedure *procedure, const TreeData &instr_data)
{
  auto instr = ParseInstruction(instr_data, procedure->GetFilename());
  if (instr)
  {
    return procedure->PushInstruction(instr.release());
  }
  return false;
}

}  // unnamed namespace

}  // namespace sequencer

}  // namespace sup
