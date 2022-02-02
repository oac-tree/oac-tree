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

// Global header files

#include <chrono>
#include <thread>

#include <common/log-api.h>

// Local header files

#include "Constants.h"
#include "Procedure.h"
#include "Runner.h"
#include "UserInterface.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

// Global variables

// Function declaration

namespace sup
{
namespace sequencer
{
static int TickTimeoutMs(Procedure* procedure);

// Function definition

Runner::Runner(UserInterface* ui)
  : _proc{nullptr}
  , _ui{ui}
  , halt{false}
{}

Runner::~Runner() = default;

void Runner::SetProcedure(Procedure* procedure)
{
  _proc = procedure;
  UserInterface* ui = _ui;
  _proc->AddUpdateCallback(
    [ui](const std::string& name, const ccs::types::AnyValue& value)
    {
      ui->VariableUpdated(name, value);
    });
}

void Runner::ExecuteProcedure()
{
  halt.store(false);
  if (_proc)
  {
    auto sleep_time_ms = TickTimeoutMs(_proc);

    while (!IsFinished() && !halt.load())
    {
      ExecuteSingle();
      if (IsRunning())
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_ms));
      }
    }
  }
}

void Runner::ExecuteSingle()
{
  if (_proc)
  {
    _ui->StartSingleStep();
    _proc->ExecuteSingle(_ui);
    _ui->EndSingleStep();
  }
}

void Runner::Halt()
{
  halt.store(true);
  if (_proc)
  {
    _proc->Halt();
  }
}

bool Runner::IsFinished() const
{
  if (!_proc)
  {
    return true;
  }

  auto status = _proc->GetStatus();
  return (status == ExecutionStatus::SUCCESS || status == ExecutionStatus::FAILURE);
}

bool Runner::IsRunning() const
{
  if (!_proc)
  {
    return false;
  }

  auto status = _proc->GetStatus();
  return (status == ExecutionStatus::RUNNING);
}

static int TickTimeoutMs(Procedure* procedure)
{
  if (procedure->HasAttribute(Procedure::TICK_TIMEOUT_ATTRIBUTE_NAME))
  {
    auto tick_timeout_str = procedure->GetAttribute(Procedure::TICK_TIMEOUT_ATTRIBUTE_NAME);
    double tick_timeout = std::stod(tick_timeout_str);
    if (tick_timeout > 0.001)
    {
      return static_cast<int>(tick_timeout * 1000);
    }
  }
  return DefaultSettings::DEFAULT_SLEEP_TIME_MS;
}

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C

#undef LOG_ALTERN_SRC
