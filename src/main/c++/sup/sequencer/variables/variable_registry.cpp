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

#include <sup/sequencer/variable_registry.h>

#include "file_variable.h"
#include "local_variable.h"

#include <sup/sequencer/log.h>
#include <sup/sequencer/variable.h>

#include <mutex>

namespace sup
{
namespace sequencer
{
std::once_flag global_variable_registry_initialized_flag;

void InitVariableRegistry(VariableRegistry& registry);

VariableRegistry& GlobalVariableRegistry()
{
  static VariableRegistry global_variable_registry;
  std::call_once(global_variable_registry_initialized_flag, InitVariableRegistry,
                 std::ref(global_variable_registry));
  return global_variable_registry;
}

bool VariableRegistry::RegisterVariable(std::string name, VariableConstructor constructor)
{
  _variable_map[name] = constructor;
  return true;
}

std::unique_ptr<Variable> VariableRegistry::Create(std::string name)
{
  auto entry = _variable_map.find(name);
  if (entry == _variable_map.end())
  {
    log::Error("VariableRegistry::Create('%s') - Variable not registered", name.c_str());
    return {};
  }
  return std::unique_ptr<Variable>(entry->second());
}

std::vector<std::string> VariableRegistry::RegisteredVariableNames() const
{
  std::vector<std::string> result;
  for (const auto elem : _variable_map)
  {
    result.push_back(elem.first);
  }
  return result;
}

void InitVariableRegistry(VariableRegistry& registry)
{
  (void)RegisterVariable<LocalVariable>(registry);
  (void)RegisterVariable<FileVariable>(registry);
}

}  // namespace sequencer

}  // namespace sup
