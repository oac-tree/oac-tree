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

#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

namespace sup
{
namespace sequencer
{
const std::string Input::Type = "Input";

ExecutionStatus Input::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  if (!HasAttribute("output"))
  {
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue value;
  if (!ws->GetValue(GetAttribute("output"), value))
  {
    return ExecutionStatus::FAILURE;
  }
  if (!ui->GetUserValue(value, GetAttribute("description")))
  {
    return ExecutionStatus::FAILURE;
  }
  if (!ws->SetValue(GetAttribute("output"), value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

Input::Input() : Instruction(Input::Type) {}

Input::~Input() = default;

}  // namespace sequencer

}  // namespace sup
