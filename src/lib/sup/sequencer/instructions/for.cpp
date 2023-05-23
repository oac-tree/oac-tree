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

const std::string ARRAY_ATTR_NAME = "arrayVar";
const std::string ELEMENT_ATTR_NAME = "elementVar";

namespace sup
{
namespace sequencer
{
const std::string ForInstruction::Type = "For";

ForInstruction::ForInstruction()
  : DecoratorInstruction(ForInstruction::Type)
  , m_count{0}
{
  AddAttributeDefinition(ARRAY_ATTR_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(ELEMENT_ATTR_NAME, sup::dto::StringType).SetMandatory();
}

ForInstruction::~ForInstruction() = default;

void ForInstruction::InitHook()
{
  m_count = 0;
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
    return ExecutionStatus::FAILURE;
  }
  if (!dto::IsArrayValue(array))
  {
    std::string warning_message =
      InstructionWarningProlog(*this) + "For instruction expects an array but variable [" +
      GetAttributeValue<std::string>(ARRAY_ATTR_NAME) + "] is not one.";
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
  if (i.GetType() != array[m_count].GetType())
  {
    std::string warning_message =
      InstructionWarningProlog(*this) + "The element [" +
      GetAttributeValue<std::string>(ELEMENT_ATTR_NAME) + "] and the elements of array [" +
      GetAttributeValue<std::string>(ARRAY_ATTR_NAME) + "] have to be of the same type.";
    ui.LogWarning(warning_message);
    return ExecutionStatus::FAILURE;
  }

  ws.SetValue(GetAttributeValue<std::string>(ELEMENT_ATTR_NAME),array[m_count]);

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
    m_count++;
  }
  return CalculateStatus(max_count);
}

ExecutionStatus ForInstruction::CalculateStatus(int max_count) const
{
  auto child_status = GetChildStatus();
  if (child_status == ExecutionStatus::SUCCESS)
  {
    if (m_count == max_count)
    {
      return ExecutionStatus::SUCCESS;
    }
    return ExecutionStatus::NOT_FINISHED;
  }
  return child_status;
}

}  // namespace sequencer

}  // namespace sup
