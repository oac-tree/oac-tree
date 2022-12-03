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
  if (!HasAttribute(OUTPUT_VARIABLE_ATTR_NAME))
  {
    std::string error_message =
      "sup::sequencer::Input::SetupImpl(): missing mandatory attribute [" +
       OUTPUT_VARIABLE_ATTR_NAME + "]";
    throw InstructionSetupException(error_message);
  }
}

ExecutionStatus Input::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  sup::dto::AnyValue value;
  if (!ws->GetValue(GetAttribute(OUTPUT_VARIABLE_ATTR_NAME), value))
  {
    return ExecutionStatus::FAILURE;
  }
  if (!ui->GetUserValue(value, GetAttribute("description")))
  {
    return ExecutionStatus::FAILURE;
  }
  if (!ws->SetValue(GetAttribute(OUTPUT_VARIABLE_ATTR_NAME), value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
