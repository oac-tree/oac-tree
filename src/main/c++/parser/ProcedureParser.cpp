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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

// Global header files
#include <common/log-api.h>
#include <common/AnyTypeDatabase.h>

// Local header files

#include "ProcedureParser.h"
#include "InstructionParser.h"
#include "SequenceParser.h"
#include "VariableParser.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

static const std::string PROCEDURE_ELEMENT_NAME = "Procedure";
static const std::string WORKSPACE_ELEMENT_NAME = "Workspace";
static const std::string PLUGIN_ELEMENT_NAME = "Plugin";
static const std::string REGISTERTYPE_ELEMENT_NAME = "RegisterType";
static const std::string JSONTYPE_ATTRIBUTE_NAME = "jsontype";

// Function declaration

static bool ParseAndLoadPlugins(const TreeData & data);
static bool ParseAndLoadPlugin(const TreeData & child);
static bool ParseTypeRegistration(const TreeData & data);
static bool RegisterTypeInformation(const TreeData & child);
static bool ParseProcedureChildren(Procedure * procedure, const TreeData & data);
static bool AddWorkspaceVariables(Procedure * procedure, const TreeData & ws_data);
static bool ParseAndAddInstruction(Procedure * procedure, const TreeData & instr_data);

// Function definition

std::unique_ptr<Procedure> ParseProcedure(const TreeData & data, const std::string & filename)
{
  log_info("sup::sequencer::ParseProcedure() - entering function..");

  if (data.GetType() != PROCEDURE_ELEMENT_NAME)
  {
    log_warning("sup::sequencer::ParseProcedure() - incorrect root element type: '%s'",
                data.GetType().c_str());
    return {};
  }

  auto result = std::unique_ptr<Procedure>(new Procedure());

  // Add current filename
  result->SetFilename(filename);

  // Load plugins first
  bool status = ParseAndLoadPlugins(data);

  // Parse type registration
  if (status)
  {
    status = ParseTypeRegistration(data);
  }

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

std::string GetFullPathName(const std::string & directory, const std::string & filename)
{
  log_info("sup::sequencer::GetFullPathName(%s, %s) - entering function..",
           directory.c_str(), filename.c_str());
  if (filename.empty())
  {
    log_warning("sup::sequencer::GetFullPathName() - empty filename as argument");
    return {};
  }
  if (filename.front() == '/')
  {
    return filename;
  }
  return directory + filename;
}

std::string GetFileDirectory(const std::string & filename)
{
  auto pos = filename.find_last_of("/");
  if (pos == std::string::npos)
  {
    return {};
  }
  return filename.substr(0, pos + 1);
}

static bool ParseAndLoadPlugins(const TreeData & data)
{
  bool result = true;
  for (const auto & child : data.Children())
  {
    if (child.GetType() == PLUGIN_ELEMENT_NAME)
    {
      log_info("ParseAndLoadPlugins() - Parsing plugin information..");
      if (!ParseAndLoadPlugin(child))
      {
        log_warning("ParseAndLoadPlugins() - Couldn't parse or load plugin data..");
        result = false;
      }
    }
  }
  return result;
}

static bool ParseAndLoadPlugin(const TreeData & child)
{
  auto plugin_name = child.GetContent();
  if (plugin_name.empty())
  {
    return true;
  }
  log_info("ParseAndLoadPlugin() - parsing plugin '%s'", plugin_name.c_str());
  bool success = LoadPlugin(plugin_name);
  if (!success)
  {
    log_warning("ParseAndLoadPlugin() - could not load plugin '%s'", plugin_name.c_str());
  }
  return success;
}

static bool ParseTypeRegistration(const TreeData & data)
{
  bool result = true;
  for (const auto & child : data.Children())
  {
    if (child.GetType() == REGISTERTYPE_ELEMENT_NAME)
    {
      log_info("ParseTypeRegistration() - Parsing type registration..");
      if (!RegisterTypeInformation(child))
      {
        log_warning("ParseTypeRegistration() - Couldn't parse type registration..");
        result = false;
      }
    }
  }
  return result;
}

static bool RegisterTypeInformation(const TreeData & child)
{
  bool status = child.HasAttribute(JSONTYPE_ATTRIBUTE_NAME);
  if (status)
  {
    log_info("RegisterTypeInformation() - function called with json type '%s' ..",
             child.GetAttribute(JSONTYPE_ATTRIBUTE_NAME).c_str());
    status = ::ccs::base::GlobalTypeDatabase::Register(child.GetAttribute(JSONTYPE_ATTRIBUTE_NAME).c_str());
  }
  return status;
}

static bool ParseProcedureChildren(Procedure * procedure, const TreeData & data)
{
  bool status = true;
  for (auto it = data.Children().begin(); status && it != data.Children().end(); ++it)
  {
    auto element_type = it->GetType();
    if (element_type == WORKSPACE_ELEMENT_NAME)
    {
      status = AddWorkspaceVariables(procedure, *it);
    }
    else if (element_type == PLUGIN_ELEMENT_NAME || element_type == REGISTERTYPE_ELEMENT_NAME)
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

static bool AddWorkspaceVariables(Procedure * procedure, const TreeData & ws_data)
{
  bool result = true;
  log_info("AddWorkspaceVariables() - generating workspace variables..");
  for (auto &var_data : ws_data.Children())
  {
    auto name = var_data.GetName();
    log_info("AddWorkspaceVariables() - generate variable: '%s'", name.c_str());
    if (!name.empty())
    {
      auto var = ParseVariable(var_data);
      if(!var)
      {
        return false;
      }
      result = procedure->AddVariable(name, var.release()) && result;
    }
  }
  return result;
}

static bool ParseAndAddInstruction(Procedure * procedure, const TreeData & instr_data)
{
  auto instr = ParseInstruction(instr_data, procedure->GetFilename());
  if (instr)
  {
    return procedure->PushInstruction(instr.release());
  }
  return false;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

}// extern C

#undef LOG_ALTERN_SRC
