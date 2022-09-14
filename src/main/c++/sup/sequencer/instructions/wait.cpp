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
#include <sup/sequencer/log.h>

#include <common/TimeTools.h>

#include <chrono>
#include <cmath>
#include <thread>

namespace sup
{
namespace sequencer
{
const std::string Wait::Type = "Wait";

static unsigned long ToNanoSeconds(double sec);

void Wait::InitHook()
{
  unsigned long _now = ::ccs::HelperTools::GetCurrentTime();
  _finish = _now + _timeout;
}

ExecutionStatus Wait::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  (void)ui;
  (void)ws;
  while (!_halt_requested.load() && _finish > ::ccs::HelperTools::GetCurrentTime())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(DefaultSettings::TIMING_ACCURACY_MS));
  }
  if (_halt_requested.load())
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

bool Wait::SetupImpl(const Procedure& /*proc*/)
{
  if (HasAttribute("timeout"))
  {
    auto timeout = GetAttribute("timeout");
    try
    {
      double t = std::stod(timeout);
      _timeout = ToNanoSeconds(t);
    }
    catch (const std::exception&)
    {
      log::Warning("Wait::SetupImpl() - could not parse timeout attribute!");
    }
  }
  return true;  // if timeout was not specified, Wait immediately returns SUCCESS.
}

Wait::Wait() : Instruction(Type), _timeout(0), _finish(0) {}

Wait::~Wait() = default;

static unsigned long ToNanoSeconds(double sec)
{
  if (sec > 0.0)
  {
    return static_cast<unsigned long>(std::lround(sec * 1e9));
  }
  return 0;
}

}  // namespace sequencer

}  // namespace sup
