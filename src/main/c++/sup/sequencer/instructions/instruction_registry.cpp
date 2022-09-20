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

#include <sup/sequencer/instruction_registry.h>

#include "choice.h"
#include "condition.h"
#include "copy.h"
#include "equals.h"
#include "fallback.h"
#include "force_success.h"
#include "include.h"
#include "input.h"
#include "inverter.h"
#include "listen.h"
#include "message.h"
#include "output.h"
#include "parallel_sequence.h"
#include "repeat.h"
#include "reset_variable.h"
#include "sequence.h"
#include "user_choice.h"
#include "wait.h"

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/log.h>

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
  auto it = _instruction_map.find(name);
  if (it != _instruction_map.end())
  {
    throw std::runtime_error("Already registered instruction with the name '"+name+"'");
  }
  _instruction_map.insert(it, {name, constructor});
  return true;
}

std::unique_ptr<Instruction> InstructionRegistry::Create(std::string name)
{
  auto entry = _instruction_map.find(name);
  if (entry == _instruction_map.end())
  {
    log::Error("InstructionRegistry::Create('%s') - Instruction not registered", name.c_str());
    return {};
  }
  log::Debug("InstructionRegistry::Create('%s') - Create instruction ..", name.c_str());
  return std::unique_ptr<Instruction>(entry->second());
}

std::vector<std::string> InstructionRegistry::RegisteredInstructionNames() const
{
  std::vector<std::string> result;
  for (const auto& elem : _instruction_map)
  {
    result.push_back(elem.first);
  }
  return result;
}

void InitInstructionRegistry(InstructionRegistry& registry)
{
  // Register compound instructions:
  (void)RegisterInstruction<Fallback>(registry);
  (void)RegisterInstruction<ParallelSequence>(registry);
  (void)RegisterInstruction<Sequence>(registry);
  (void)RegisterInstruction<UserChoice>(registry);

  // Register decorator instructions:
  (void)RegisterInstruction<ForceSuccess>(registry);
  (void)RegisterInstruction<Include>(registry);
  (void)RegisterInstruction<Inverter>(registry);
  (void)RegisterInstruction<Listen>(registry);
  (void)RegisterInstruction<Repeat>(registry);

  // Register leaf instructions:
  // (void)RegisterInstruction<Choice>(registry);
  (void)RegisterInstruction<Condition>(registry);
  (void)RegisterInstruction<Copy>(registry);
  (void)RegisterInstruction<Equals>(registry);
  (void)RegisterInstruction<Input>(registry);
  (void)RegisterInstruction<Message>(registry);
  (void)RegisterInstruction<Output>(registry);
  (void)RegisterInstruction<ResetVariable>(registry);
  (void)RegisterInstruction<Wait>(registry);
  // (void)RegisterInstruction<WaitForUpdateInstruction>(registry);
}

}  // namespace sequencer

}  // namespace sup
