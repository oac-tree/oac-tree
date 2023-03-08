/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) Sequencer component
 *
 * Description   : Instruction node implementation
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "equals.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

const std::string LEFT_VARIABLE_ATTR_NAME = "lhs";
const std::string RIGHT_VARIABLE_ATTR_NAME = "rhs";

namespace sup
{
namespace sequencer
{
const std::string Equals::Type = "Equals";

Equals::Equals() : Instruction(Equals::Type) {}
Equals::~Equals() = default;

void Equals::SetupImpl(const Procedure &proc)
{
  CheckMandatoryNonEmptyAttribute(*this, LEFT_VARIABLE_ATTR_NAME);
  CheckMandatoryNonEmptyAttribute(*this, RIGHT_VARIABLE_ATTR_NAME);
}

ExecutionStatus Equals::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  auto left_field = GetAttribute(LEFT_VARIABLE_ATTR_NAME);
  auto right_field = GetAttribute(RIGHT_VARIABLE_ATTR_NAME);
  auto left_var = SplitFieldName(left_field).first;
  auto right_var = SplitFieldName(right_field).first;
  if (!ws->HasVariable(left_var))
  {
    std::string error_message = InstructionErrorLogProlog() +
      "workspace does not contain left hand side variable with name [" + left_var + "]";
    ui->LogError(error_message);
    return ExecutionStatus::FAILURE;
  }
  if (!ws->HasVariable(right_var))
  {
    std::string error_message = InstructionErrorLogProlog() +
      "workspace does not contain right hand side variable with name [" + right_var + "]";
    ui->LogError(error_message);
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue lhs;
  if (!ws->GetValue(left_field, lhs))
  {
    std::string warning_message = InstructionWarningLogProlog() +
      "could not read left field with name [" + left_field + "] from workspace";
    ui->LogWarning(warning_message);
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue rhs;
  if (!ws->GetValue(right_field, rhs))
  {
    std::string warning_message = InstructionWarningLogProlog() +
      "could not read right field with name [" + right_field + "] from workspace";
    ui->LogWarning(warning_message);
    return ExecutionStatus::FAILURE;
  }
  return lhs == rhs ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup
