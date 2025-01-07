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
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "async_wrapper.h"

#include <chrono>
#include <functional>

namespace sup
{
namespace sequencer
{
AsyncWrapper::AsyncWrapper(Instruction* instruction)
  : m_instruction{instruction}
  , m_status{ExecutionStatus::NOT_STARTED}
{}

AsyncWrapper::AsyncWrapper(AsyncWrapper&&) = default;

AsyncWrapper::~AsyncWrapper() = default;

void AsyncWrapper::Tick(UserInterface& ui, Workspace& ws)
{
  // Only query instruction status and possibly launch child instruction when there's no
  // unfinished thread.
  if (!WaitingForThread())
  {
    auto status_changed = UpdateStatus();
    if (!status_changed && NeedsExecute(m_status))
    {
      LaunchChild(ui, ws);
      m_status = ExecutionStatus::RUNNING;
    }
  }
}

ExecutionStatus AsyncWrapper::GetStatus() const
{
  return m_status;
}

const Instruction* AsyncWrapper::GetInstruction() const
{
   return m_instruction;
}

bool AsyncWrapper::WaitingForThread() const
{
  if (!m_child_result.valid())
  {
    return false;
  }
  if (m_child_result.wait_for(std::chrono::seconds(0)) == std::future_status::timeout)
  {
    return true;
  }
  return false;
}

bool AsyncWrapper::UpdateStatus()
{
  auto old_status = m_status;
  m_status = m_instruction->GetStatus();
  return m_status != old_status;
}

void AsyncWrapper::LaunchChild(UserInterface& ui, Workspace& ws)
{
  m_child_result = std::async(std::launch::async, &Instruction::ExecuteSingle,
                              m_instruction, std::ref(ui), std::ref(ws));
}

}  // namespace sequencer

}  // namespace sup
