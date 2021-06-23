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

#include "DecoratorInstruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

void DecoratorInstruction::ResetHook()
{
  ResetChild();
}

void DecoratorInstruction::HaltImpl()
{
  HaltChild();
}

std::vector<const Instruction *> DecoratorInstruction::ChildInstructionsImpl() const
{
  std::vector<const Instruction *> result;
  if (_child)
  {
    result.push_back(_child.get());
  }
  return result;
}

bool DecoratorInstruction::SetupImpl(const Procedure & proc)
{
  return SetupChild(proc);
}

bool DecoratorInstruction::SetupChild(const Procedure & proc)
{
  log_debug("DecoratorInstruction::SetupChild - entering function..");
  bool status = true;
  if (_child)
  {
    status = _child->Setup(proc);
  }
  return status;
}

bool DecoratorInstruction::HasChild() const
{
  return static_cast<bool>(_child);
}

ExecutionStatus DecoratorInstruction::GetChildStatus() const
{
  if (_child)
  {
    return _child->GetStatus();
  }
  return ExecutionStatus::NOT_STARTED;
}

void DecoratorInstruction::ExecuteChild(UserInterface *ui, Workspace *ws)
{
  if (_child)
  {
    _child->ExecuteSingle(ui, ws);
  }
}

void DecoratorInstruction::ResetChild()
{
  if (_child)
  {
    _child->Reset();
  }
}

void DecoratorInstruction::HaltChild()
{
  if (_child)
  {
    _child->Halt();
  }
}

DecoratorInstruction::DecoratorInstruction(const std::string & type)
  : Instruction(type)
  , _child{}
{}

DecoratorInstruction::~DecoratorInstruction()
{}

void DecoratorInstruction::SetInstruction(Instruction * instruction)
{
  _child.reset(instruction);
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
