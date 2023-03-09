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

#include "wait.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/generic_utils.h>

#include <chrono>
#include <cmath>
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
{}

Wait::~Wait() = default;

void Wait::SetupImpl(const Procedure&)
{
  if (HasAttribute(TIMEOUT_ATTR_NAME))
  {
    double t = InstructionAttributeToDouble(*this, TIMEOUT_ATTR_NAME);
    if (t < 0.0 || t > MAX_TIMEOUT_SECONDS)
    {
        std::string error_message = InstructionSetupExceptionProlog(*this) +
          "could not convert timeout in seconds [" + std::to_string(t) +
          "] to nanoseconds (64bit unsigned)";
        throw InstructionSetupException(error_message);
    }
    m_timeout = static_cast<unsigned long>(std::lround(t * 1e9));
  }
}

ExecutionStatus Wait::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  (void)ui;
  (void)ws;
  auto finish = utils::GetNanosecsSinceEpoch() + m_timeout;
  while (!_halt_requested.load() && finish > utils::GetNanosecsSinceEpoch())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(DefaultSettings::TIMING_ACCURACY_MS));
  }
  if (_halt_requested.load())
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
