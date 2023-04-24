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

#include "async_wrapper.h"

#include <chrono>
#include <functional>

namespace sup
{
namespace sequencer
{
AsyncWrapper::AsyncWrapper(Instruction* instruction)
  : _instruction{instruction}
  , _status{ExecutionStatus::NOT_STARTED}
{}

AsyncWrapper::AsyncWrapper(AsyncWrapper&&) = default;

AsyncWrapper::~AsyncWrapper() = default;

void AsyncWrapper::Tick(UserInterface& ui, Workspace& ws)
{
  if (ChildIsRunning())
  {
    return;
  }
  auto child_status = _instruction->GetStatus();

  if (child_status == ExecutionStatus::SUCCESS || child_status == ExecutionStatus::FAILURE)
  {
    _status = child_status;
  }
  else
  {
    LaunchChild(ui, ws);
    _status = ExecutionStatus::RUNNING;
  }
}

ExecutionStatus AsyncWrapper::GetStatus() const
{
  return _status;
}

void AsyncWrapper::LaunchChild(UserInterface& ui, Workspace& ws)
{
  _child_result = std::async(std::launch::async, &Instruction::ExecuteSingle,
                             _instruction, std::ref(ui), std::ref(ws));
}

bool AsyncWrapper::ChildIsRunning() const
{
  if (!_child_result.valid())
  {
    return false;
  }
  auto result_status = _child_result.wait_for(std::chrono::seconds(0));
  return result_status == std::future_status::timeout;
}

}  // namespace sequencer

}  // namespace sup
