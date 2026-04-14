/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) oac-tree component
 *
 * Description   : Instruction node implementation
 *
 * Author        : Ricardo Torres (EXT)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "fail.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/instruction_utils.h>
#include <sup/oac-tree/generic_utils.h>
#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/user_interface.h>

#include <chrono>

namespace sup
{
namespace oac_tree
{

const std::string Fail::Type = "Fail";

Fail::Fail()
  : Instruction(Fail::Type)
  , m_blocking{false}
  , m_finish{}
  , m_mtx{}
  , m_cv{}
{
  (void)AddAttributeDefinition(Constants::TIMEOUT_SEC_ATTRIBUTE_NAME, sup::dto::Float64Type)
    .SetCategory(AttributeCategory::kBoth);
  (void)AddAttributeDefinition(Constants::BLOCKING_ATTRIBUTE_NAME, sup::dto::BooleanType)
    .SetCategory(AttributeCategory::kBoth);
}

Fail::~Fail() = default;

bool Fail::InitHook(UserInterface& ui, Workspace& ws)
{
  if (!GetAttributeValueAs(Constants::BLOCKING_ATTRIBUTE_NAME, ws, ui, m_blocking))
  {
    return false;
  }
  sup::dto::uint64 timeout_ns{};
  if (!instruction_utils::GetVariableTimeoutAttribute(
            *this, ui, ws, Constants::TIMEOUT_SEC_ATTRIBUTE_NAME, timeout_ns))
  {
    return false;
  }
  m_finish = utils::GetNanosecsSinceEpoch() + timeout_ns;
  return true;
}

ExecutionStatus Fail::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  (void)ui;
  (void)ws;
  auto now = utils::GetNanosecsSinceEpoch();
  if (m_blocking)
  {
    if (now < m_finish)
    {
      std::unique_lock<std::mutex> lk{m_mtx};
      auto pred = [this]() { return IsHaltRequested(); };
      (void)m_cv.wait_for(lk, std::chrono::nanoseconds(m_finish - now), pred);
    }
  }
  else
  {
    if (!IsHaltRequested() && (m_finish > now))
    {
      return ExecutionStatus::RUNNING;
    }
  }
  return ExecutionStatus::FAILURE;
}

void Fail::HaltImpl(UserInterface& ui)
{
  (void)ui;
  std::lock_guard<std::mutex> lock(m_mtx);  // Avoid lost wakeup
  m_cv.notify_one();
}

void Fail::ResetHook(UserInterface& ui)
{
  (void)ui;
  m_blocking = false;
  m_finish = 0;
}

}  // namespace oac_tree

}  // namespace sup
