/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Ricardo Torres (EXT)
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

#include "for.h"
#include "sup/sequencer/execution_status.h"
#include "sup/sequencer/instruction.h"
#include "sup/sequencer/user_interface.h"
#include <sup/dto/basic_scalar_types.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/generic_utils.h>

const std::string ELEMENT_ATTR_NAME = "elementVar";
const std::string ARRAY_ATTR_NAME = "arrayVar";

namespace sup
{
namespace sequencer
{
const std::string ForInstruction::Type = "For";

ForInstruction::ForInstruction()
  : DecoratorInstruction(ForInstruction::Type)
  , _count{0}
{}

ForInstruction::~ForInstruction() = default;

void ForInstruction::InitHook()
{
  _count = 0;
}

void ForInstruction::SetupImpl(const Procedure& proc)
{
  CheckMandatoryNonEmptyAttribute(*this, ELEMENT_ATTR_NAME);
  CheckMandatoryNonEmptyAttribute(*this, ARRAY_ATTR_NAME);

  SetupChild(proc);
}

ExecutionStatus ForInstruction::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (!HasChild())
  {
    return ExecutionStatus::SUCCESS;
  }
  sup::dto::AnyValue array;
  if (!GetValueFromAttributeName(*this, ws, ui, ARRAY_ATTR_NAME, array))
  {
    std::string warning_message = InstructionWarningProlog(*this)
                                  + "Failed to retriece the value of ["
                                  + GetAttribute(ARRAY_ATTR_NAME) + "].";
    ui.LogWarning(warning_message);
    return ExecutionStatus::FAILURE;
  }
  if (!dto::IsArrayValue(array))
  {
    std::string warning_message = InstructionWarningProlog(*this)
                                  + "For instruction expects an array but variable ["
                                  + GetAttribute(ARRAY_ATTR_NAME) + "] is not one.";
    ui.LogWarning(warning_message);
    return ExecutionStatus::FAILURE;
  }

  int max_count = array.NumberOfElements();
  if (max_count == 0)
  {
    return ExecutionStatus::SUCCESS;
  }

  dto::AnyValue i;
  GetValueFromAttributeName(*this, ws, ui, ELEMENT_ATTR_NAME, i);
  if (i.GetType() != array[_count].GetType())
  {
    std::string warning_message = InstructionWarningProlog(*this) + "The element ["
                                  + GetAttribute(ELEMENT_ATTR_NAME)
                                  + "] and the elements of array [" + GetAttribute(ARRAY_ATTR_NAME)
                                  + "] have to be of the same type.";
    ui.LogWarning(warning_message);
    return ExecutionStatus::FAILURE;
  }

  ws.SetValue(GetAttribute(ELEMENT_ATTR_NAME),array[_count]);

  auto child_status = GetChildStatus();
  if (child_status == ExecutionStatus::SUCCESS)
  {
    ResetChild();
  }
  ExecuteChild(ui, ws);

  child_status = GetChildStatus();

  // Don't increment count when _max_count is not strictly positive.
  if (max_count > 0
      && (child_status == ExecutionStatus::SUCCESS || child_status == ExecutionStatus::FAILURE))
  {
    _count++;
  }
  return CalculateStatus(max_count);
}

ExecutionStatus ForInstruction::CalculateStatus(int max_count) const
{
  auto child_status = GetChildStatus();
  if (child_status == ExecutionStatus::SUCCESS)
  {
    if (_count == max_count)
    {
      return ExecutionStatus::SUCCESS;
    }
    return ExecutionStatus::NOT_FINISHED;
  }
  return child_status;
}

}  // namespace sequencer

}  // namespace sup
