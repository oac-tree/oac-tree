/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "wait.h"

#include "sup/oac-tree/procedure.h"
#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_utils.h>
#include <sup/oac-tree/generic_utils.h>
#include <sup/oac-tree/user_interface.h>

#include <chrono>
#include <thread>

namespace sup
{
namespace oac_tree
{
const std::string Wait::Type = "Wait";

Wait::Wait()
  : Instruction(Wait::Type)
{
  AddAttributeDefinition(Constants::TIMEOUT_SEC_ATTRIBUTE_NAME, sup::dto::Float64Type)
    .SetCategory(AttributeCategory::kBoth);
}

Wait::~Wait() = default;

void Wait::SetupImpl(const Procedure& proc)
{
  m_timing_accuracy = TimingAccuracyMs(proc);
}

ExecutionStatus Wait::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::int64 timeout_ns;
  if (!instruction_utils::GetVariableTimeoutAttribute(
            *this, ui, ws, Constants::TIMEOUT_SEC_ATTRIBUTE_NAME, timeout_ns))
  {
    return ExecutionStatus::FAILURE;
  }
  auto finish = utils::GetNanosecsSinceEpoch() + timeout_ns;
  while (!IsHaltRequested() && finish > utils::GetNanosecsSinceEpoch())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(m_timing_accuracy));
  }
  if (IsHaltRequested())
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace oac_tree

}  // namespace sup
