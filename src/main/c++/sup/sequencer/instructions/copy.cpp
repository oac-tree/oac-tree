/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) Sequencer component
 *
 * Description   : Instruction node implementation
 *
 * Author        : Bertrand Bauvir (IO)
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

#include "copy.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/log_severity.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue_helper.h>

const std::string INPUT_VARIABLE_ATTR_NAME = "input";
const std::string OUTPUT_VARIABLE_ATTR_NAME = "output";

namespace sup
{
namespace sequencer
{
const std::string Copy::Type = "Copy";

Copy::Copy() : Instruction(Copy::Type) {}
Copy::~Copy() = default;

void Copy::SetupImpl(const Procedure &proc)
{
  if (!HasAttribute(INPUT_VARIABLE_ATTR_NAME) || !HasAttribute(OUTPUT_VARIABLE_ATTR_NAME))
  {
    std::string error_message =
      "sup::sequencer::Copy::SetupImpl(): missing mandatory attributes [" +
       INPUT_VARIABLE_ATTR_NAME + ", " + OUTPUT_VARIABLE_ATTR_NAME + "]";
    throw InstructionSetupException(error_message);
  }
}

ExecutionStatus Copy::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  auto input_field_name = GetAttribute(INPUT_VARIABLE_ATTR_NAME);
  auto output_field_name = GetAttribute(OUTPUT_VARIABLE_ATTR_NAME);
  auto input_var_name = SplitFieldName(input_field_name).first;
  auto output_var_name = SplitFieldName(output_field_name).first;
  if (!ws->HasVariable(input_var_name))
  {
    std::string error_message =
      "sup::sequencer::Copy::ExecuteSingleImpl(): workspace does not contain input variable with "
      "name [" + input_var_name + "]";
    ui->Log(log::SUP_SEQ_LOG_ERR, error_message);
    return ExecutionStatus::FAILURE;
  }
  if (!ws->HasVariable(output_var_name))
  {
    std::string error_message =
      "sup::sequencer::Copy::ExecuteSingleImpl(): workspace does not contain output variable with "
      "name [" + output_var_name + "]";
    ui->Log(log::SUP_SEQ_LOG_ERR, error_message);
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue value;
  if (!ws->GetValue(input_field_name, value))
  {
    std::string warning_message =
      "sup::sequencer::Copy::ExecuteSingleImpl(): could not read input field with name [" +
      input_field_name + "] from workspace";
    ui->Log(log::SUP_SEQ_LOG_WARNING, warning_message);
    return ExecutionStatus::FAILURE;
  }
  if (!ws->SetValue(output_field_name, value))
  {
    auto json_value = sup::dto::ValuesToJSONString(value);
    std::string warning_message =
      "sup::sequencer::Copy::ExecuteSingleImpl(): could not copy value [" + json_value +
      "] to workspace field with name [" + output_field_name + "]";
    ui->Log(log::SUP_SEQ_LOG_WARNING, warning_message);
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
