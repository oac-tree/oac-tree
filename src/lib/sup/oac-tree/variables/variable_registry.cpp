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

#include <sup/oac-tree/variable_registry.h>

#include "file_variable.h"
#include "local_variable.h"

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/variable.h>

#include <mutex>

namespace sup
{
namespace oac_tree
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

bool VariableRegistry::RegisterVariable(std::string name, VariableConstructor constructor,
                                        DocumentationEntry doc)
{
  auto it = m_variable_entries.find(name);
  if (it != m_variable_entries.end())
  {
    std::string error_message =
      "VariableRegistry::RegisterVariable(): trying to register variable "
      "with name [" + name + "] twice";
    throw InvalidOperationException(error_message);
  }
  (void)m_variable_entries.emplace(name, VariableEntry{constructor, doc});
  return true;
}

std::unique_ptr<Variable> VariableRegistry::Create(std::string name)
{
  auto entry = m_variable_entries.find(name);
  if (entry == m_variable_entries.end())
  {
    std::string error_message =
      "VariableRegistry::Create(): trying to create unregistered variable "
      "with name [" + name + "]";
    throw InvalidOperationException(error_message);
  }
  return std::unique_ptr<Variable>(entry->second.ctor());
}

DocumentationEntry VariableRegistry::GetDocumentation(const std::string& name)
{
  auto entry = m_variable_entries.find(name);
  if (entry == m_variable_entries.end())
  {
    std::string error_message =
      "InstructionRegistry::GetDocumentation(): variable "
      "with name [" + name + "] not found";
    throw InvalidOperationException(error_message);
  }
  return entry->second.doc;
}

void VariableRegistry::SetDocumentationField(
  const std::string& name, const std::string& field_name, const std::string& field_val)
{
  auto entry = m_variable_entries.find(name);
  if (entry == m_variable_entries.end())
  {
    std::string error_message =
      "VariableRegistry::SetDocumentationField(): variable "
      "with name [" + name + "] not found";
    throw InvalidOperationException(error_message);
  }
  entry->second.doc.SetField(field_name, field_val);
}

std::vector<std::string> VariableRegistry::RegisteredVariableNames() const
{
  std::vector<std::string> result;
  for (const auto& [variable_name, _] : m_variable_entries)
  {
    result.push_back(variable_name);
  }
  return result;
}

bool VariableRegistry::IsRegisteredVariableName(const std::string& name) const
{
  auto it = m_variable_entries.find(name);
  return it != m_variable_entries.end();
}

void InitVariableRegistry(VariableRegistry& registry)
{
  (void)RegisterVariable<LocalVariable>(registry);
  (void)RegisterVariable<FileVariable>(registry);
}

}  // namespace oac_tree

}  // namespace sup
