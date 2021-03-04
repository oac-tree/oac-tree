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

#include "Include.h"
#include "InstructionHelper.h"
#include "Procedure.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

const std::string PATH_ATTRIBUTE_NAME="path";

// Type definition

namespace sup {

namespace sequencer {

// Global variables

const std::string Include::Type = "Include";

// Function declaration

// Function definition

ExecutionStatus Include::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
  if (!_child)
  {
    return ExecutionStatus::SUCCESS;
  }

  auto child_status = _child->GetStatus();
  if (NeedsExecute(child_status))
  {
    _child->ExecuteSingle(ui, ws);
  }

  return CalculateStatus();
}

bool Include::SetupImpl(const Procedure & proc)
{
  auto instructions = proc.GetInstructions();
  auto path = GetPath();
  auto instr = InstructionHelper::FindInstruction(instructions, path);
  if (instr == nullptr)
  {
    log_warning("Include::SetupImpl(): instruction with path '%s' not found",
                path.c_str());
    return false;
  }
  std::unique_ptr<Instruction> clone(InstructionHelper::CloneInstruction(instr));
  if (InstructionHelper::InitialiseVariableAttributes(*clone, GetAttributes()))
  {
    SetInstruction(clone.release());
    return true;
  }
  log_warning("Include::SetupImpl(): instruction with path '%s' could not be ",
              "properly initialised with the given attributes", path.c_str());
  return false;
}

std::string Include::GetPath() const
{
  return GetAttribute(PATH_ATTRIBUTE_NAME);
}

ExecutionStatus Include::CalculateStatus() const
{
  if (!_child)
  {
    return ExecutionStatus::SUCCESS;
  }

  return _child->GetStatus();
}

Include::Include()
  : DecoratorInstruction(Type)
{}

Include::~Include()
{}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
