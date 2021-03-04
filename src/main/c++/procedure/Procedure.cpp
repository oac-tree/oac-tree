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
#include <algorithm>

// Local header files

#include "Procedure.h"
#include "Sequence.h"
#include "Workspace.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

namespace sup {

namespace sequencer {

const std::string Procedure::TICK_TIMEOUT_ATTRIBUTE_NAME = "tickTimeout";

// Type definition

// Global variables

static const std::string IS_ROOT_ATTRIBUTE="isRoot";
static const std::vector<std::string> TRUE_LIST = {
    "True", "true", "TRUE",
    "Yes", "yes", "YES"
};

// Function declaration

static bool HasRootAttributeSet(const Instruction & instruction);

// Function definition

Procedure::Procedure()
  : _instructions{}
  , _workspace{new Workspace()}
{}

Procedure::~Procedure() = default;

Instruction * Procedure::RootInstrunction()
{
  // Scott Meyers' solution for avoiding code duplication:
  return const_cast<Instruction *>(static_cast<const Procedure &>(*this).RootInstrunction());
}

const Instruction * Procedure::RootInstrunction() const
{
  if (_instructions.size() == 0)
  {
    return nullptr;
  }
  if (_instructions.size() == 1)
  {
    return _instructions[0].get();
  }
  for (auto & instr : _instructions)
  {
    if (HasRootAttributeSet(*instr))
    {
      return instr.get();
    }
  }
  return nullptr;
}

std::vector<const Instruction *> Procedure::GetInstructions() const
{
  std::vector<const Instruction *> result;
  for (auto & instr : _instructions)
  {
    result.push_back(instr.get());
  }
  return result;
}

bool Procedure::PushInstruction(Instruction * instruction)
{
  if (instruction == nullptr)
  {
    return false;
  }
  _instructions.emplace_back(instruction);
  return true;
}

bool Procedure::AddVariable(std::string name, Variable * var)
{
  return _workspace->AddVariable(name, var);
}

std::vector<std::string> Procedure::VariableNames() const
{
  return _workspace->VariableNames();
}

bool Procedure::GetVariableValue(std::string name, ::ccs::types::AnyValue & value)
{
  return _workspace->GetValue(name, value);
}

bool Procedure::Setup()
{
  if (RootInstrunction() == nullptr)
  {
    return true;
  }
  return RootInstrunction()->Setup(*this);
}

void Procedure::ExecuteSingle(UserInterface * ui)
{
  if (RootInstrunction() == nullptr)
  {
    return;
  }
  RootInstrunction()->ExecuteSingle(ui, _workspace.get());
}

ExecutionStatus Procedure::GetStatus() const
{
  if (RootInstrunction() == nullptr)
  {
    return ExecutionStatus::NOT_STARTED;
  }
  return RootInstrunction()->GetStatus();
}

bool Procedure::HasAttribute(const std::string & name) const
{
  return _attributes.HasAttribute(name);
}

std::string Procedure::GetAttribute(const std::string & name) const
{
  return _attributes.GetAttribute(name);
}

bool Procedure::AddAttribute(const std::string & name, const std::string & value)
{
  return _attributes.AddAttribute(name, value);
}

static bool HasRootAttributeSet(const Instruction & instruction)
{
  if (!instruction.HasAttribute(IS_ROOT_ATTRIBUTE))
  {
    return false;
  }
  auto attr_val = instruction.GetAttribute(IS_ROOT_ATTRIBUTE);
  auto it = std::find(TRUE_LIST.begin(), TRUE_LIST.end(), attr_val);
  return it != TRUE_LIST.end();
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

}// extern C

#undef LOG_ALTERN_SRC
