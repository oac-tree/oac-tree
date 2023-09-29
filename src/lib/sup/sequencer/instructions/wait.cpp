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

#include "wait.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_utils.h>
#include <sup/sequencer/generic_utils.h>
#include <sup/sequencer/user_interface.h>

#include <chrono>
#include <thread>

const std::string TIMEOUT_ATTR_NAME = "timeout";

const double MAX_TIMEOUT_SECONDS = 18.4e9;  // More than 500 years. This should be enough...

namespace sup
{
namespace sequencer
{
const std::string Wait::Type = "Wait";

Wait::Wait()
  : Instruction(Wait::Type)
{
  AddAttributeDefinition(TIMEOUT_ATTR_NAME, sup::dto::Float64Type);
}

Wait::~Wait() = default;

ExecutionStatus Wait::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::float64 timeout_sec = 0.0;
  if (HasAttribute(TIMEOUT_ATTR_NAME) &&
      !GetVariableAttributeAs(TIMEOUT_ATTR_NAME, ws, ui, timeout_sec))
  {
    return ExecutionStatus::FAILURE;
  }
  sup::dto::int64 timeout_ns;
  if (!instruction_utils::ConvertToTimeoutNanoseconds(timeout_sec, timeout_ns))
  {
    const std::string warning_message = InstructionWarningProlog(*this) + "could not retrieve " +
      "timeout value within limits: " + std::to_string(timeout_sec);
    ui.LogWarning(warning_message);
    return ExecutionStatus::FAILURE;
  }
  auto finish = utils::GetNanosecsSinceEpoch() + timeout_ns;
  while (!IsHaltRequested() && finish > utils::GetNanosecsSinceEpoch())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(DefaultSettings::TIMING_ACCURACY_MS));
  }
  if (IsHaltRequested())
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
