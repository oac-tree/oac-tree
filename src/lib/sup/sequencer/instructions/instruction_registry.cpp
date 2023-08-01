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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/sequencer/instruction_registry.h>

#include "choice.h"
#include "condition.h"
#include "copy.h"
#include "equals.h"
#include "decrement.h"
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
#include "repeat.h"
#include "reset_variable.h"
#include "sequence.h"
#include "user_choice.h"
#include "user_confirmation.h"
#include "wait.h"
#include "waitforvariable.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction.h>

#include <mutex>

namespace sup
{
namespace sequencer
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
      "sup::sequencer::InstructionRegistry::RegisterInstruction(): trying to register instruction "
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
      "sup::sequencer::InstructionRegistry::Create(): trying to create unregistered instruction "
      "with name [" + name + "]";
    throw InvalidOperationException(error_message);
  }
  return std::unique_ptr<Instruction>(entry->second());
}

std::vector<std::string> InstructionRegistry::RegisteredInstructionNames() const
{
  std::vector<std::string> result;
  for (const auto& elem : m_instruction_map)
  {
    result.push_back(elem.first);
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
  (void)RegisterInstruction<Sequence>(registry);
  (void)RegisterInstruction<UserChoice>(registry);

  // Register decorator instructions:
  (void)RegisterInstruction<ForInstruction>(registry);
  (void)RegisterInstruction<ForceSuccess>(registry);
  (void)RegisterInstruction<Include>(registry);
  (void)RegisterInstruction<Inverter>(registry);
  (void)RegisterInstruction<Listen>(registry);
  (void)RegisterInstruction<Repeat>(registry);

  // Register leaf instructions:
  (void)RegisterInstruction<Condition>(registry);
  (void)RegisterInstruction<Copy>(registry);
  (void)RegisterInstruction<Decrement>(registry);
  (void)RegisterInstruction<Equals>(registry);
  (void)RegisterInstruction<GreaterThan>(registry);
  (void)RegisterInstruction<GreaterThanOrEqual>(registry);
  (void)RegisterInstruction<IncludeProcedure>(registry);
  (void)RegisterInstruction<Increment>(registry);
  (void)RegisterInstruction<Input>(registry);
  (void)RegisterInstruction<LessThan>(registry);
  (void)RegisterInstruction<LessThanOrEqual>(registry);
  (void)RegisterInstruction<LogInstruction>(registry);
  (void)RegisterInstruction<Message>(registry);
  (void)RegisterInstruction<Output>(registry);
  (void)RegisterInstruction<ResetVariable>(registry);
  (void)RegisterInstruction<UserConfirmation>(registry);
  (void)RegisterInstruction<Wait>(registry);
  (void)RegisterInstruction<WaitForVariable>(registry);
}

}  // namespace sequencer

}  // namespace sup
