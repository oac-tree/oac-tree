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

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

Procedure::Procedure()
  : _root_sequence{new sup::sequencer::Sequence()}
  , _workspace{}
{}

Procedure::~Procedure() = default;

bool Procedure::AddVariable(Variable var)
{
  return _workspace->AddVariable(var);
}

std::vector<std::string> Procedure::VariableNames() const
{
  return _workspace->VariableNames();
}

int Procedure::GetVariableValue(std::string name)
{
  return _workspace->GetVariableValue(name);
}

bool Procedure::SetVariableValue(std::string name, int value)
{
  return _workspace->SetVariableValue(name, value);
}

bool Procedure::PushInstruction(sup::sequencer::Instruction * instruction)
{
  _root_sequence->PushBack(instruction);
  return true;
}

void Procedure::ExecuteSingle(UserInterface * ui)
{
  _root_sequence->ExecuteSingle(ui, _workspace.get());
}

ExecutionStatus Procedure::GetStatus() const
{
  return _root_sequence->GetStatus();
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
