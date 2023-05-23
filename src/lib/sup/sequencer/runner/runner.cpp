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

#include <sup/sequencer/runner.h>

#include <sup/sequencer/constants.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/user_interface.h>

#include <chrono>
#include <thread>

namespace sup
{
namespace sequencer
{
static int TickTimeoutMs(Procedure& procedure);

Runner::Runner(UserInterface& ui)
  : m_proc{nullptr}
  , m_ui{ui}
  , m_halt{false}
{}

Runner::~Runner() = default;

void Runner::SetProcedure(Procedure* procedure)
{
  m_proc = procedure;
  m_proc->RegisterGenericCallback(
    [this](const std::string& name, const sup::dto::AnyValue& value, bool connected)
    {
      m_ui.VariableUpdated(name, value, connected);
    });
}

void Runner::ExecuteProcedure()
{
  m_halt.store(false);
  if (m_proc)
  {
    auto sleep_time_ms = TickTimeoutMs(*m_proc);

    while (!IsFinished() && !m_halt.load())
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
  if (m_proc)
  {
    m_ui.StartSingleStep();
    m_proc->ExecuteSingle(m_ui);
    m_ui.EndSingleStep();
  }
}

void Runner::Halt()
{
  m_halt.store(true);
  if (m_proc)
  {
    m_proc->Halt();
  }
}

bool Runner::IsFinished() const
{
  if (!m_proc)
  {
    return true;
  }

  auto status = m_proc->GetStatus();
  return (status == ExecutionStatus::SUCCESS || status == ExecutionStatus::FAILURE);
}

bool Runner::IsRunning() const
{
  if (!m_proc)
  {
    return false;
  }

  auto status = m_proc->GetStatus();
  return (status == ExecutionStatus::RUNNING);
}

static int TickTimeoutMs(Procedure& procedure)
{
  if (procedure.HasAttribute(kTickTimeoutAttributeName))
  {
    auto tick_timeout = procedure.GetAttributeValue<double>(kTickTimeoutAttributeName);
    if (tick_timeout > 0.001)
    {
      return static_cast<int>(tick_timeout * 1000);
    }
  }
  return DefaultSettings::DEFAULT_SLEEP_TIME_MS;
}

}  // namespace sequencer

}  // namespace sup
