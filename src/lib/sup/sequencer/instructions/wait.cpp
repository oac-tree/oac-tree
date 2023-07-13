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
  , m_timeout(0)
{
  AddAttributeDefinition(TIMEOUT_ATTR_NAME, sup::dto::Float64Type);
}

Wait::~Wait() = default;

void Wait::SetupImpl(const Procedure&)
{
  m_timeout = 0;
  if (HasAttribute(TIMEOUT_ATTR_NAME))
  {
    m_timeout = instruction_utils::GetTimeoutFromAttribute(*this, TIMEOUT_ATTR_NAME);
  }
}

ExecutionStatus Wait::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  (void)ui;
  (void)ws;
  auto finish = utils::GetNanosecsSinceEpoch() + m_timeout;
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
