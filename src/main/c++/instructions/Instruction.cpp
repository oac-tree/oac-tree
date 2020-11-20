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
#include "runner/UserInterface.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

static const std::string NAME_ATTRIBUTE = "name";

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

void Instruction::Preamble(UserInterface * ui)
{
  if (_status == ExecutionStatus::UNDEFINED)
  {
    _status = ExecutionStatus::STARTED;
    ui->UpdateInstructionStatus(this);
  }
}

void Instruction::Postamble(UserInterface * ui)
{
  if (_status != _status_before)
  {
    ui->UpdateInstructionStatus(this);
  }
}

Instruction::Instruction(std::string type)
  : _type{std::move(type)}
  , _status{ExecutionStatus::UNDEFINED}
  , _status_before{ExecutionStatus::UNDEFINED}
{}

Instruction::~Instruction() = default;

std::string Instruction::GetType() const
{
  return _type;
}

std::string Instruction::GetName() const
{
  return GetAttribute(NAME_ATTRIBUTE);
}

void Instruction::SetName(std::string name)
{
  SetAttribute(NAME_ATTRIBUTE, name);
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
  _status = ExecutionStatus::UNDEFINED;
}

std::string Instruction::GetAttribute(const std::string & name) const
{
  if (_attributes.find(name) == _attributes.end())
  {
      return {};
  }
  return _attributes.at(name);
}

std::string Instruction::SetAttribute(const std::string & name, const std::string & value)
{
  _attributes[name] = value;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
