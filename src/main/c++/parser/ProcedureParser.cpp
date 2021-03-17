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

static const std::string WORKSPACE_ELEMENT_NAME = "Workspace";
static const std::string PLUGIN_ELEMENT_NAME = "Plugin";

// Function declaration

static bool ParseAndLoadPlugins(const TreeData & data);
static bool ParseAndLoadPlugin(const TreeData & child);
static bool AddWorkspaceVariables(Procedure * procedure, const TreeData & ws_data);
static bool ParseAndAddInstruction(Procedure * procedure, const TreeData & instr_data);

// Function definition

std::unique_ptr<Procedure> ParseProcedure(const TreeData & data, const std::string & filename)
{
  log_info("sup::sequencer::ParseProcedure() - entering function..");

  auto result = std::unique_ptr<Procedure>(new Procedure());

  // Add current filename
  result->SetFilename(filename);

  // Load plugins first
  ParseAndLoadPlugins(data);

  // Add attributes
  result->AddAttributes(data.Attributes());

  // Parse child elements
  for (auto & child : data.Children())
  {
    if (child.GetType() == WORKSPACE_ELEMENT_NAME)
    {
      AddWorkspaceVariables(result.get(), child);
    }
    else if (child.GetType() == PLUGIN_ELEMENT_NAME)
    {
      continue;  // Plugins were already handled.
    }
    else
    {
      // Every non workspace element of the Procedure node should be an instruction node
      ParseAndAddInstruction(result.get(), child);
    }
  }
  return result;
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
