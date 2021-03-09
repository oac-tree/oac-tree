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

static std::string GetFileDirectory(const std::string & filename);
static bool ParseAndLoadPlugins(const TreeData & data);

// Function definition

std::unique_ptr<Procedure> ParseProcedure(const TreeData & data, const std::string & filename)
{
  log_info("sup::sequencer::ParseProcedure() - entering function..");

  auto result = std::unique_ptr<Procedure>(new Procedure());

  // Add current directory if filename not empty
  if (!filename.empty())
  {
    auto file_directory = GetFileDirectory(filename);
    log_info("ParseProcedure() - setting current directory to '%s'", file_directory.c_str());
    result->SetCurrentDirectory(file_directory);
  }

  // Add attributes
  for (const auto & attr : data.Attributes())
  {
    result->AddAttribute(attr.first, attr.second);
  }

  // Load plugins first
  for (const auto &child : data.Children())
  {
    if (child.GetType() == PLUGIN_ELEMENT_NAME)
    {
      log_info("sup::sequencer::ParseProcedure() - Parsing plugin information..");
      if (!ParseAndLoadPlugins(child))
      {
        log_warning("sup::sequencer::ParseProcedure() - Couldn't parse or load plugin data..");
      }
    }
  }
  for (const auto &child : data.Children())
  {
    if (child.GetType() == WORKSPACE_ELEMENT_NAME)
    {
      log_info("sup::sequencer::ParseProcedure() - generating workspace variables..");
      for (const auto &var_data : child.Children())
      {
        auto name = var_data.GetName();
        log_info("sup::sequencer::ParseProcedure() - generate variable: '%s'", name.c_str());
        if (!name.empty())
        {
          auto var = ParseVariable(var_data);
          result->AddVariable(name, var.release());
        }
      }
    }
    else if (child.GetType() == PLUGIN_ELEMENT_NAME)
    {
      continue;
    }
    // Every non workspace element of the Procedure node should be an instruction node
    else
    {
      auto root_instr = ParseInstruction(child, result->GetCurrentDirectory());
      if (root_instr)
      {
        result->PushInstruction(root_instr.release());
      }
    }
  }
  return result;
}

static std::string GetFileDirectory(const std::string & filename)
{
  auto pos = filename.find_last_of("/");
  if (pos == std::string::npos)
  {
    return {};
  }
  return filename.substr(0, pos);
}

static bool ParseAndLoadPlugins(const TreeData &child) {
    auto plugin_name = child.GetContent();
    if (plugin_name.empty()) {
        return true;
    }
    log_info("sup::sequencer::ParseProcedure() - parsing plugin '%s'", plugin_name.c_str());
    bool success = LoadPlugin(plugin_name);
    if (!success) {
        log_warning("sup::sequencer::ParseProcedure() - could not load plugin '%s'", plugin_name.c_str());
    }
    return success;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

}// extern C

#undef LOG_ALTERN_SRC
