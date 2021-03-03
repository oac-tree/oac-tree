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

#include "Instruction.h"
#include "UserInterface.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

void Instruction::InitHook()
{}

void Instruction::Preamble(UserInterface * ui)
{
  PreExecuteHook(ui);
  if (_status == ExecutionStatus::NOT_STARTED)
  {
    InitHook();
    _status = ExecutionStatus::NOT_FINISHED;
    ui->UpdateInstructionStatus(this);
  }
}

void Instruction::PreExecuteHook(UserInterface * ui)
{
  (void)ui;
}

void Instruction::PostExecuteHook(UserInterface * ui)
{
  (void)ui;
}

void Instruction::Postamble(UserInterface * ui)
{
  if (_status != _status_before)
  {
    ui->UpdateInstructionStatus(this);
  }
  PostExecuteHook(ui);
}

void Instruction::ResetHook()
{}

bool Instruction::SetupImpl(const Procedure & proc)
{
  return true;
}

Instruction::Instruction(const std::string & type)
  : _type{type}
  , _status{ExecutionStatus::NOT_STARTED}
  , _status_before{ExecutionStatus::NOT_STARTED}
{}

Instruction::~Instruction() = default;

std::string Instruction::GetType() const
{
  return _type;
}

std::string Instruction::GetName() const
{
  return GetAttribute(attributes::NAME_ATTRIBUTE);
}

void Instruction::SetName(const std::string & name)
{
  AddAttribute(attributes::NAME_ATTRIBUTE, name);
}

bool Instruction::Setup(const Procedure & proc)
{
  return SetupImpl(proc);
}

void Instruction::ExecuteSingle(UserInterface * ui, Workspace * ws)
{
  Preamble(ui);

  _status_before = _status;

  _status = ExecuteSingleImpl(ui, ws);

  Postamble(ui);
}

ExecutionStatus Instruction::GetStatus() const
{
  return _status;
}

void Instruction::ResetStatus()
{
  if (_status == ExecutionStatus::RUNNING)
  {
    // log warning: instructions in RUNNING status should not receive this call!
    return;
  }
  ResetHook();
  _status = ExecutionStatus::NOT_STARTED;
}

bool Instruction::HasAttribute(const std::string & name) const
{
  return _attributes.HasAttribute(name);
}

std::string Instruction::GetAttribute(const std::string & name) const
{
  return _attributes.GetAttribute(name);
}

AttributeMap Instruction::GetAttributes() const
{
  return _attributes;
}

bool Instruction::AddAttribute(const std::string & name, const std::string & value)
{
  return _attributes.AddAttribute(name, value);
}

bool NeedsExecute(ExecutionStatus status)
{
  return (status == ExecutionStatus::NOT_STARTED ||
          status == ExecutionStatus::NOT_FINISHED ||
          status == ExecutionStatus::RUNNING);
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
