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

#include <mutex>

#include <common/log-api.h>

// Local header files

#include "FileVariable.h"
#include "LocalVariable.h"
#include "Variable.h"
#include "VariableRegistry.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup
{
namespace sequencer
{
// Global variables

std::once_flag global_variable_registry_initialized_flag;

// Function declaration

void InitVariableRegistry(VariableRegistry& registry);

// Function definition

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
    log_error("VariableRegistry::Create('%s') - Variable not registered", name.c_str());
    return {};
  }
  log_debug("VariableRegistry::Create('%s') - Create variable ..", name.c_str());
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

extern "C"
{
  // C API function definitions

}  // extern C

#undef LOG_ALTERN_SRC
