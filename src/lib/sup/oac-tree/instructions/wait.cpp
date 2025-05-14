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
  , m_blocking{false}
  , m_timing_accuracy_ns{}
  , m_finish{}
{
  AddAttributeDefinition(Constants::TIMEOUT_SEC_ATTRIBUTE_NAME, sup::dto::Float64Type)
    .SetCategory(AttributeCategory::kBoth);
  AddAttributeDefinition(Constants::BLOCKING_ATTRIBUTE_NAME, sup::dto::BooleanType)
    .SetCategory(AttributeCategory::kBoth);
}

Wait::~Wait() = default;

void Wait::SetupImpl(const Procedure& proc)
{
  m_timing_accuracy_ns = TimingAccuracyNs(proc);
}

bool Wait::InitHook(UserInterface& ui, Workspace& ws)
{
  if (!GetAttributeValueAs(Constants::BLOCKING_ATTRIBUTE_NAME, ws, ui, m_blocking))
  {
    return false;
  }
  sup::dto::int64 timeout_ns;
  if (!instruction_utils::GetVariableTimeoutAttribute(
            *this, ui, ws, Constants::TIMEOUT_SEC_ATTRIBUTE_NAME, timeout_ns))
  {
    return false;
  }
  m_finish = utils::GetNanosecsSinceEpoch() + timeout_ns;
  return true;
}

ExecutionStatus Wait::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  (void)ui;
  (void)ws;
  if (m_blocking)
  {
    while (!IsHaltRequested() && m_finish > utils::GetNanosecsSinceEpoch())
    {
      std::this_thread::sleep_for(std::chrono::nanoseconds(m_timing_accuracy_ns));
    }
  }
  auto now = utils::GetNanosecsSinceEpoch();
  if (!IsHaltRequested() && m_finish > now)
  {
    return ExecutionStatus::RUNNING;
  }
  if (IsHaltRequested())
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

void Wait::ResetHook(UserInterface& ui)
{
  (void)ui;
  m_blocking = false;
  m_timing_accuracy_ns = 0;
  m_finish = 0;
}

}  // namespace oac_tree

}  // namespace sup
