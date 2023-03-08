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

#include "input.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue_helper.h>

const std::string OUTPUT_VARIABLE_ATTR_NAME = "output";

namespace sup
{
namespace sequencer
{
const std::string Input::Type = "Input";

Input::Input() : Instruction(Input::Type) {}

Input::~Input() = default;

void Input::SetupImpl(const Procedure &proc)
{
  CheckMandatoryNonEmptyAttribute(*this, OUTPUT_VARIABLE_ATTR_NAME);
}

ExecutionStatus Input::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  auto output_field = GetAttribute(OUTPUT_VARIABLE_ATTR_NAME);
  auto output_var = SplitFieldName(output_field).first;
  if (!ws->HasVariable(output_var))
  {
    std::string error_message = InstructionErrorLogProlog() +
      "workspace does not contain output variable with name [" + output_var + "]";
    ui->LogError(error_message);
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue value;
  if (!ws->GetValue(output_field, value))
  {
    std::string error_message = InstructionErrorLogProlog() +
      "workspace could not retrieve value of output field with name [" + output_field + "]";
    ui->LogError(error_message);
    return ExecutionStatus::FAILURE;
  }
  if (!ui->GetUserValue(value, GetAttribute("description")))
  {
    std::string warning_message = InstructionWarningLogProlog() +
      "did not receive compatible user value for field [" + output_field + "[ in workspace";
    ui->LogWarning(warning_message);
    return ExecutionStatus::FAILURE;
  }
  if (!ws->SetValue(output_field, value))
  {
    auto json_value = sup::dto::ValuesToJSONString(value);
    std::string warning_message = InstructionWarningLogProlog() +
      "could not write user value [" + json_value + "] to field [" + output_field +
      "[ in workspace";
    ui->LogWarning(warning_message);
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
