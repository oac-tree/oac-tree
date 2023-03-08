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

#include "condition.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue.h>

const std::string CONDITION_VARIABLE_ATTR_NAME = "varName";

namespace sup
{
namespace sequencer
{
const std::string Condition::Type = "Condition";

Condition::Condition()
  : Instruction(Condition::Type)
{}

Condition::~Condition() = default;

void Condition::SetupImpl(const Procedure &proc)
{
  CheckMandatoryNonEmptyAttribute(*this, CONDITION_VARIABLE_ATTR_NAME);
}

ExecutionStatus Condition::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  auto field_name = GetAttribute(CONDITION_VARIABLE_ATTR_NAME);
  auto var_name = SplitFieldName(field_name).first;
  if (!ws->HasVariable(var_name))
  {
    std::string error_message = InstructionErrorLogProlog() +
      "workspace does not contain condition variable with name [" + var_name + "]";
    ui->LogError(error_message);
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue var;
  sup::dto::boolean result = false;
  if (!ws->GetValue(field_name, var) || !var.As(result))
  {
    std::string warning_message = InstructionWarningLogProlog() +
      "could not parse workspace field with name [" + field_name + "] to a boolean";
    ui->LogWarning(warning_message);
    return ExecutionStatus::FAILURE;
  }
  return result ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup
