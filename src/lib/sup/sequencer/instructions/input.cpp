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

#include "input.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue_helper.h>

const std::string OUTPUT_VARIABLE_ATTR_NAME = "outputVar";
const std::string DESCRIPTION_ATTR_NAME = "description";

namespace sup
{
namespace sequencer
{
const std::string Input::Type = "Input";

Input::Input()
  : Instruction(Input::Type)
{
  AddAttributeDefinition(OUTPUT_VARIABLE_ATTR_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  AddAttributeDefinition(DESCRIPTION_ATTR_NAME)
    .SetCategory(AttributeCategory::kBoth);
}

Input::~Input() = default;

ExecutionStatus Input::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue value;
  if (!GetAttributeValue(OUTPUT_VARIABLE_ATTR_NAME, ws, ui, value))
  {
    return ExecutionStatus::FAILURE;
  }
  std::string description;
  if (!GetAttributeValueAs(DESCRIPTION_ATTR_NAME, ws, ui, description))
  {
    return ExecutionStatus::FAILURE;
  }
  if (!ui.GetUserValue(value, description))
  {
    std::string warning_message = InstructionWarningProlog(*this) +
      "did not receive compatible user value for field [" +
      GetAttributeString(OUTPUT_VARIABLE_ATTR_NAME) + "[ in workspace";
    ui.LogWarning(warning_message);
    return ExecutionStatus::FAILURE;
  }
  if (!SetValueFromAttributeName(*this, ws, ui, OUTPUT_VARIABLE_ATTR_NAME, value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
