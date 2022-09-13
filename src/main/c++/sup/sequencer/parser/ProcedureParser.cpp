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

#include "ProcedureParser.h"

#include "InstructionParser.h"
#include "VariableParser.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/log.h>
#include <sup/sequencer/sequence_parser.h>

#include <common/AnyTypeDatabase.h>

#include <fstream>
#include <sstream>

namespace sup
{
namespace sequencer
{
// Global variables

static const std::string JSONTYPE_ATTRIBUTE_NAME = "jsontype";
static const std::string JSONFILE_ATTRIBUTE_NAME = "jsonfile";

// Function declaration

static std::string ReadJSONFile(const std::string &filename);
static bool ParsePreamble(const TreeData &data, const std::string &filename);
static bool ParseAndLoadPlugin(const TreeData &child);
static bool RegisterTypeInformation(const TreeData &child, const std::string &filename);
static bool ParseProcedureChildren(Procedure *procedure, const TreeData &data);
static bool AddWorkspaceVariables(Procedure *procedure, const TreeData &ws_data);
static bool ParseAndAddInstruction(Procedure *procedure, const TreeData &instr_data);

// Function definition

std::unique_ptr<Procedure> ParseProcedure(const TreeData &data, const std::string &filename)
{
  log::Debug("sup::sequencer::ParseProcedure() - entering function..");

  if (data.GetType() != Constants::PROCEDURE_ELEMENT_NAME)
  {
    log::Warning("sup::sequencer::ParseProcedure() - incorrect root element type: '%s'",
                data.GetType().c_str());
    return {};
  }

  auto result = std::unique_ptr<Procedure>(new Procedure());

  // Add current filename
  result->SetFilename(filename);

  // Load plugins and register types first
  bool status = ParsePreamble(data, filename);

  // Add attributes
  result->AddAttributes(data.Attributes());

  // Parse child elements
  if (status)
  {
    status = ParseProcedureChildren(result.get(), data);
  }
  if (status)
  {
    return result;
  }
  return {};
}

std::string GetFullPathName(const std::string &directory, const std::string &filename)
{
  log::Debug("sup::sequencer::GetFullPathName(%s, %s) - entering function..", directory.c_str(),
            filename.c_str());
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

static std::string ReadJSONFile(const std::string &filename)
{
  if (!::ccs::HelperTools::Exist(filename.c_str()))
  {
    log::Warning("ReadJSONFile('%s') - file not found", filename.c_str());
    return {};
  }
  std::ifstream ifstr(filename);
  std::string result;
  while (!ifstr.eof())
  {
    std::string tmp;
    ifstr >> tmp;
    result.append(tmp);
  }
  return result;
}

static bool ParsePreamble(const TreeData &data, const std::string &filename)
{
  bool result = true;
  for (const auto &child : data.Children())
  {
    if (child.GetType() == Constants::PLUGIN_ELEMENT_NAME)
    {
      if (!ParseAndLoadPlugin(child))
      {
        result = false;
      }
    }
    else if (child.GetType() == Constants::REGISTERTYPE_ELEMENT_NAME)
    {
      if (!RegisterTypeInformation(child, filename))
      {
        result = false;
      }
    }
  }
  return result;
}

static bool ParseAndLoadPlugin(const TreeData &child)
{
  auto plugin_name = child.GetContent();
  if (plugin_name.empty())
  {
    return true;
  }
  log::Debug("ParseAndLoadPlugin() - parsing plugin '%s'", plugin_name.c_str());
  bool success = LoadPlugin(plugin_name);
  if (!success)
  {
    log::Warning("ParseAndLoadPlugin() - could not load plugin '%s'", plugin_name.c_str());
  }
  return success;
}

static bool RegisterTypeInformation(const TreeData &child, const std::string &filename)
{
  bool status = true;
  if (child.HasAttribute(JSONTYPE_ATTRIBUTE_NAME))
  {
    log::Debug("RegisterTypeInformation() - function called with json type '%s' ..",
              child.GetAttribute(JSONTYPE_ATTRIBUTE_NAME).c_str());
    status = ::ccs::base::GlobalTypeDatabase::Register(
        child.GetAttribute(JSONTYPE_ATTRIBUTE_NAME).c_str());
  }
  else if (child.HasAttribute(JSONFILE_ATTRIBUTE_NAME))
  {
    log::Debug("RegisterTypeInformation() - function called with json file '%s' ..",
              child.GetAttribute(JSONFILE_ATTRIBUTE_NAME).c_str());
    std::string jsonfile =
        GetFullPathName(GetFileDirectory(filename), child.GetAttribute(JSONFILE_ATTRIBUTE_NAME));
    auto type_string = ReadJSONFile(jsonfile);
    log::Debug("RegisterTypeInformation() - json file '%s', type string '%s' ..", jsonfile.c_str(),
              type_string.c_str());
    status = ::ccs::base::GlobalTypeDatabase::Register(type_string.c_str());
  }
  if (!status)
  {
    log::Warning("RegisterTypeInformation() - could not register type");
  }
  return status;
}

static bool ParseProcedureChildren(Procedure *procedure, const TreeData &data)
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

static bool AddWorkspaceVariables(Procedure *procedure, const TreeData &ws_data)
{
  bool result = true;
  log::Debug("AddWorkspaceVariables() - generating workspace variables..");
  for (auto &var_data : ws_data.Children())
  {
    auto name = var_data.GetName();
    log::Debug("AddWorkspaceVariables() - generate variable: '%s'", name.c_str());
    if (!name.empty())
    {
      auto var = ParseVariable(var_data);
      if (!var)
      {
        return false;
      }
      result = procedure->AddVariable(name, var.release()) && result;
    }
  }
  return result;
}

static bool ParseAndAddInstruction(Procedure *procedure, const TreeData &instr_data)
{
  auto instr = ParseInstruction(instr_data, procedure->GetFilename());
  if (instr)
  {
    return procedure->PushInstruction(instr.release());
  }
  return false;
}

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C
