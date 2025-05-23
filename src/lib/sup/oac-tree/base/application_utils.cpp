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

#include <sup/oac-tree/application_utils.h>

#include <sup/oac-tree/generic_utils.h>
#include <sup/oac-tree/sequence_parser.h>

#include <chrono>

namespace sup
{
namespace oac_tree
{

namespace utils
{

std::unique_ptr<Procedure> SafeParseProcedure(const std::string& filename, std::string& error_msg)
{
  error_msg.clear();
  if (!utils::FileExists(filename))
  {
    error_msg = "Procedure file not found [" + filename + "]";
    return {};
  }
  try
  {
    auto proc = ParseProcedureFile(filename);
    if (!proc)
    {
      error_msg = "Could not parse procedure file [" + filename + "]";
      return {};
    }
    return proc;
  }
  catch(const std::exception& e)
  {
    error_msg = e.what();
  }
  return {};
}

std::unique_ptr<AsyncRunner> CreateAsyncRunner(Procedure& proc, UserInterface& ui,
                                                   JobStateMonitor& state_monitor,
                                                   std::string& error_msg)
{
  std::unique_ptr<AsyncRunner> result;
  error_msg.clear();
  try
  {
    result = std::make_unique<AsyncRunner>(proc, ui, state_monitor);
  }
  catch(const std::exception& e)
  {
    error_msg = e.what();
  }
  return result;
}

SimpleJobStateMonitor::SimpleJobStateMonitor()
  : m_state{JobState::kInitial}
  , m_mtx{}
  , m_cv{}
{}

SimpleJobStateMonitor::~SimpleJobStateMonitor() = default;

void SimpleJobStateMonitor::OnStateChange(JobState state) noexcept
{
  {
    std::lock_guard<std::mutex> lk{m_mtx};
    m_state = state;
  }
  m_cv.notify_one();
}

void SimpleJobStateMonitor::OnBreakpointChange(const Instruction* instruction,
                                               bool breakpoint_set) noexcept
{
  // Ignore breakpoint updates
  (void)instruction;
  (void)breakpoint_set;
}

void SimpleJobStateMonitor::OnProcedureTick(const Procedure& proc)
{
  // Ignore callbacks between ticks
  (void)proc;
}

JobState SimpleJobStateMonitor::GetCurrentState() const
{
  std::lock_guard<std::mutex> lk{m_mtx};
  return m_state;
}

JobState SimpleJobStateMonitor::WaitForFinished() const
{
  auto pred = [this](){
    return IsFinishedJobState(m_state);
  };
  std::unique_lock<std::mutex> lk{m_mtx};
  m_cv.wait(lk, pred);
  return m_state;
}

bool SimpleJobStateMonitor::WaitForState(JobState state, double seconds) const
{
  auto duration = std::chrono::duration<double>(seconds);
  std::unique_lock<std::mutex> lk{m_mtx};
  return m_cv.wait_for(lk, duration, [this, state](){
    return m_state == state;
  });
}

}  // namespace utils

}  // namespace oac_tree

}  // namespace sup
