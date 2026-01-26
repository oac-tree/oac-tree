/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "cl_utils.h"

#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/variable_registry.h>

#include <sstream>

namespace sup
{
namespace oac_tree
{
std::string GetLoadedPluginMessage()
{
  const auto instr_plugin_names = GetAllPluginNames(GlobalInstructionRegistry());
  const auto var_plugin_names = GetAllPluginNames(GlobalVariableRegistry());
  auto plugins = instr_plugin_names;
  for (const auto& var_plugin_name : var_plugin_names)
  {
    plugins.insert(var_plugin_name);
  }
  if (plugins.empty())
  {
    return kNoPluginsLoaded;
  }
  std::ostringstream oss{};
  oss << "Loaded plugins:\n";
  for (const auto& plugin : plugins)
  {
    oss << "  " << plugin << "\n";
  }
  return oss.str();
}

}  // namespace oac_tree

}  // namespace sup

