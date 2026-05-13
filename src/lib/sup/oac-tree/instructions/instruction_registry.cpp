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

#include <sup/oac-tree/instruction_registry.h>

#include "all_instructions.h"

#include <sup/oac-tree/exceptions.h>

#include <mutex>

namespace sup
{
namespace oac_tree
{
std::once_flag global_instruction_registry_initialized_flag;

void InitInstructionRegistry(InstructionRegistry& registry);

InstructionRegistry& GlobalInstructionRegistry()
{
  static InstructionRegistry global_instruction_registry;
  std::call_once(global_instruction_registry_initialized_flag, InitInstructionRegistry,
                 std::ref(global_instruction_registry));
  return global_instruction_registry;
}

bool InstructionRegistry::RegisterInstruction(std::string name, InstructionConstructor constructor,
                                              DocumentationEntry doc)
{
  (void)doc;
  auto it = m_instruction_entries.find(name);
  if (it != m_instruction_entries.end())
  {
    std::string error_message =
      "InstructionRegistry::RegisterInstruction(): trying to register instruction "
      "with name [" + name + "] twice";
    throw InvalidOperationException(error_message);
  }
  (void)m_instruction_entries.emplace(name, InstructionEntry{constructor, std::move(doc)});
  return true;
}

std::unique_ptr<Instruction> InstructionRegistry::Create(const std::string& name)
{
  auto entry = m_instruction_entries.find(name);
  if (entry == m_instruction_entries.end())
  {
    std::string error_message =
      "InstructionRegistry::Create(): trying to create unregistered instruction "
      "with name [" + name + "]";
    throw InvalidOperationException(error_message);
  }
  return std::unique_ptr<Instruction>(entry->second.ctor());
}

DocumentationEntry InstructionRegistry::GetDocumentation(const std::string& name) const
{
  auto entry = m_instruction_entries.find(name);
  if (entry == m_instruction_entries.end())
  {
    std::string error_message =
      "InstructionRegistry::GetDocumentation(): instruction "
      "with name [" + name + "] not found";
    throw InvalidOperationException(error_message);
  }
  return entry->second.doc;
}

void InstructionRegistry::SetDocumentationField(
  const std::string& name, const std::string& field_name, const std::string& field_val)
{
  auto entry = m_instruction_entries.find(name);
  if (entry == m_instruction_entries.end())
  {
    std::string error_message =
      "InstructionRegistry::SetDocumentationField(): instruction "
      "with name [" + name + "] not found";
    throw InvalidOperationException(error_message);
  }
  entry->second.doc.SetField(field_name, field_val);
}

std::vector<std::string> InstructionRegistry::RegisteredInstructionNames() const
{
  std::vector<std::string> result;
  for (const auto& [instruction_name, _] : m_instruction_entries)
  {
    result.push_back(instruction_name);
  }
  return result;
}

void SetAllEmptyDocFields(InstructionRegistry& registry, const std::string& field_name,
                          const std::string& field_val)
{
  const auto instr_names = registry.RegisteredInstructionNames();
  for (const auto& instr_name : instr_names)
  {
    if (registry.GetDocumentation(instr_name).GetField(field_name).empty())
    {
      registry.SetDocumentationField(instr_name, field_name, field_val);
    }
  }
}

std::set<std::string> GetAllPluginNames(const InstructionRegistry& registry)
{
  std::set<std::string> result{};
  const auto instructions = registry.RegisteredInstructionNames();
  for (const auto& instr : instructions)
  {
    const auto doc = registry.GetDocumentation(instr);
    const auto plugin_name = doc.GetField(Constants::INSTR_DOC_PLUGIN_NAME);
    if (!plugin_name.empty() && (plugin_name != Constants::DOC_CORE_LIBRARY_NAME))
    {
      result.insert(plugin_name);
    }
  }
  return result;
}

std::vector<std::string> GetInstructionsFromPlugin(const InstructionRegistry& registry,
                                                   const std::string& plugin_name)
{
  std::vector<std::string> result{};
  const auto instructions = registry.RegisteredInstructionNames();
  for (const auto& instr : instructions)
  {
    const auto doc = registry.GetDocumentation(instr);
    const auto plugin = doc.GetField(Constants::INSTR_DOC_PLUGIN_NAME);
    if (plugin == plugin_name)
    {
      result.push_back(instr);
    }
  }
  return result;
}

std::string GetPluginForInstruction(const InstructionRegistry& registry,
                                    const std::string& instr_name)
{
  try
  {
    const auto doc = registry.GetDocumentation(instr_name);
    return doc.GetField(Constants::INSTR_DOC_PLUGIN_NAME);
  }
  catch(const InvalidOperationException&)
  {
    // do nothing
  }
  return {};
}

bool InstructionRegistry::IsRegisteredInstructionName(const std::string& name) const
{
  auto it = m_instruction_entries.find(name);
  return it != m_instruction_entries.end();
}

void InitInstructionRegistry(InstructionRegistry& registry)
{
  // Register compound instructions:
  (void)RegisterInstruction<Choice>(registry);
  (void)RegisterInstruction<Fallback>(registry);
  (void)RegisterInstruction<ParallelSequence>(registry);
  (void)RegisterInstruction<ReactiveFallback>(registry);
  (void)RegisterInstruction<ReactiveSequence>(registry);
  (void)RegisterInstruction<Sequence>(registry);
  (void)RegisterInstruction<UserChoice>(registry);

  // Register decorator instructions:
  (void)RegisterInstruction<Async>(registry);
  (void)RegisterInstruction<ForInstruction>(registry);
  (void)RegisterInstruction<ForceSuccess>(registry);
  (void)RegisterInstruction<Include>(registry);
  (void)RegisterInstruction<IncludeProcedure>(registry);
  (void)RegisterInstruction<Inverter>(registry);
  (void)RegisterInstruction<Listen>(registry);
  (void)RegisterInstruction<Repeat>(registry);

  // Register leaf instructions:
  (void)RegisterInstruction<AddElement>(registry);
  (void)RegisterInstruction<AddMember>(registry);
  (void)RegisterInstruction<Condition>(registry);
  (void)RegisterInstruction<CopyFromProcedureInstruction>(registry);
  (void)RegisterInstruction<CopyToProcedureInstruction>(registry);
  (void)RegisterInstruction<Copy>(registry);
  (void)RegisterInstruction<Decrement>(registry);
  (void)RegisterInstruction<Equals>(registry);
  (void)RegisterInstruction<Fail>(registry);
  (void)RegisterInstruction<GreaterThan>(registry);
  (void)RegisterInstruction<GreaterThanOrEqual>(registry);
  (void)RegisterInstruction<Increment>(registry);
  (void)RegisterInstruction<Input>(registry);
  (void)RegisterInstruction<LessThan>(registry);
  (void)RegisterInstruction<LessThanOrEqual>(registry);
  (void)RegisterInstruction<LogInstruction>(registry);
  (void)RegisterInstruction<Message>(registry);
  (void)RegisterInstruction<Output>(registry);
  (void)RegisterInstruction<ResetVariable>(registry);
  (void)RegisterInstruction<ReadFile>(registry);
  (void)RegisterInstruction<Succeed>(registry);
  (void)RegisterInstruction<UserConfirmation>(registry);
  (void)RegisterInstruction<VarExistsInstruction>(registry);
  (void)RegisterInstruction<Wait>(registry);
  (void)RegisterInstruction<WaitForVariable>(registry);
  (void)RegisterInstruction<WaitForVariables>(registry);

  // Set plugin name of these instructions to 'CORE'
  SetAllEmptyDocFields(registry, Constants::INSTR_DOC_PLUGIN_NAME,
                       Constants::DOC_CORE_LIBRARY_NAME);
}

}  // namespace oac_tree

}  // namespace sup
