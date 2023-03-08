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

static unsigned long ToNanoSeconds(double sec, const std::string& exception_prolog);

Wait::Wait()
  : Instruction(Wait::Type)
  , m_timeout(0)
  , m_finish(0)
{}

Wait::~Wait() = default;

void Wait::InitHook()
{
  unsigned long _now = utils::GetNanosecsSinceEpoch();
  m_finish = _now + m_timeout;
}

void Wait::SetupImpl(const Procedure&)
{
  if (HasAttribute(TIMEOUT_ATTR_NAME))
  {
    auto timeout_str = GetAttribute(TIMEOUT_ATTR_NAME);
    double t{};
    if (!utils::SafeStringToDouble(t, timeout_str))
    {
      std::string error_message = InstructionSetupExceptionProlog(*this) +
        "could not parse attribute [" + TIMEOUT_ATTR_NAME + "], with value [" + timeout_str +
        "] to double";
      throw InstructionSetupException(error_message);
    }
    m_timeout = ToNanoSeconds(t, InstructionSetupExceptionProlog(*this));
  }
}

ExecutionStatus Wait::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  (void)ui;
  (void)ws;
  while (!_halt_requested.load() && m_finish > utils::GetNanosecsSinceEpoch())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(DefaultSettings::TIMING_ACCURACY_MS));
  }
  if (_halt_requested.load())
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

static unsigned long ToNanoSeconds(double sec, const std::string& exception_prolog)
{
  if (sec < 0.0 || sec > MAX_TIMEOUT_SECONDS)
  {
      std::string error_message = exception_prolog +
        "could not convert timeout in seconds [" + std::to_string(sec) +
        "] to nanoseconds (64bit unsigned)";
      throw InstructionSetupException(error_message);
  }
  return static_cast<unsigned long>(std::lround(sec * 1e9));
}

}  // namespace sequencer

}  // namespace sup
