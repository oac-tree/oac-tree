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

// Function declaration

// Function definition

Instruction * Procedure::Root()
{
  // Scott Meyers' solution for avoiding code duplication:
  return const_cast<Instruction *>(static_cast<const Procedure &>(*this).Root());
}

const Instruction * Procedure::Root() const
{
  if (_instructions.size() == 0)
  {
    return nullptr;
  }
  return _instructions[0].get();
}

Procedure::Procedure()
  : _instructions{}
  , _workspace{new Workspace()}
{}

Procedure::~Procedure() = default;

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

void Procedure::SetRootInstruction(Instruction * instruction)
{
  _instructions.clear();
  _instructions.emplace_back(instruction);
}

bool Procedure::PushInstruction(Instruction * instruction)
{
  if (Root() == nullptr)
  {
    _instructions.emplace_back(new Sequence{});
  }
  auto compound = dynamic_cast<CompoundInstruction *>(Root());
  if (compound)
  {
    compound->PushBack(instruction);
    return true;
  }
  return false;
}

bool Procedure::Setup()
{
  if (Root() == nullptr)
  {
    return true;
  }
  return Root()->Setup(*this);
}

void Procedure::ExecuteSingle(UserInterface * ui)
{
  if (Root() == nullptr)
  {
    return;
  }
  Root()->ExecuteSingle(ui, _workspace.get());
}

ExecutionStatus Procedure::GetStatus() const
{
  if (Root() == nullptr)
  {
    return ExecutionStatus::NOT_STARTED;
  }
  return Root()->GetStatus();
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

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

}// extern C

#undef LOG_ALTERN_SRC
