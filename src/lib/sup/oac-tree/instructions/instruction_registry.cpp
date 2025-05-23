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
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "add_element.h"
#include "add_member.h"
#include "async.h"
#include "choice.h"
#include "condition.h"
#include "copy_from_procedure.h"
#include "copy_to_procedure.h"
#include "copy.h"
#include "equals.h"
#include "var_exists.h"
#include "decrement.h"
#include "fail.h"
#include "fallback.h"
#include "for.h"
#include "force_success.h"
#include "greaterthan.h"
#include "greaterthanorequal.h"
#include "include.h"
#include "include_procedure.h"
#include "increment.h"
#include "input.h"
#include "inverter.h"
#include "lessthan.h"
#include "lessthanorequal.h"
#include "listen.h"
#include "log_instruction.h"
#include "message.h"
#include "output.h"
#include "parallel_sequence.h"
#include "reactive_fallback.h"
#include "reactive_sequence.h"
#include "repeat.h"
#include "reset_variable.h"
#include "sequence.h"
#include "succeed.h"
#include "user_choice.h"
#include "user_confirmation.h"
#include "wait.h"
#include "wait_for_variable.h"
#include "wait_for_variables.h"

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction.h>

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

bool InstructionRegistry::RegisterInstruction(std::string name, InstructionConstructor constructor)
{
  auto it = m_instruction_map.find(name);
  if (it != m_instruction_map.end())
  {
    std::string error_message =
      "InstructionRegistry::RegisterInstruction(): trying to register instruction "
      "with name [" + name + "] twice";
    throw InvalidOperationException(error_message);
  }
  m_instruction_map.insert(it, {name, constructor});
  return true;
}

std::unique_ptr<Instruction> InstructionRegistry::Create(const std::string& name)
{
  auto entry = m_instruction_map.find(name);
  if (entry == m_instruction_map.end())
  {
    std::string error_message =
      "InstructionRegistry::Create(): trying to create unregistered instruction "
      "with name [" + name + "]";
    throw InvalidOperationException(error_message);
  }
  return std::unique_ptr<Instruction>(entry->second());
}

std::vector<std::string> InstructionRegistry::RegisteredInstructionNames() const
{
  std::vector<std::string> result;
  for (const auto& [instruction_name, _] : m_instruction_map)
  {
    result.push_back(instruction_name);
  }
  return result;
}

bool InstructionRegistry::IsRegisteredInstructionName(const std::string& name) const
{
  auto it = m_instruction_map.find(name);
  return it != m_instruction_map.end();
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
  (void)RegisterInstruction<Succeed>(registry);
  (void)RegisterInstruction<UserConfirmation>(registry);
  (void)RegisterInstruction<VarExistsInstruction>(registry);
  (void)RegisterInstruction<Wait>(registry);
  (void)RegisterInstruction<WaitForVariable>(registry);
  (void)RegisterInstruction<WaitForVariables>(registry);
}

}  // namespace oac_tree

}  // namespace sup
