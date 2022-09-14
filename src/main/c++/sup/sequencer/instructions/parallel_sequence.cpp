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

#include "parallel_sequence.h"

#include <sup/sequencer/log.h>

namespace sup
{
namespace sequencer
{
const std::string ParallelSequence::Type = "ParallelSequence";

static const std::string SUCCESS_THRESHOLD_ATTRIBUTE = "successThreshold";
static const std::string FAILURE_THRESHOLD_ATTRIBUTE = "failureThreshold";

void ParallelSequence::InitHook()
{
  InitWrappers();
}

ExecutionStatus ParallelSequence::ExecuteSingleImpl(UserInterface *ui, Workspace *ws)
{
  if (!HasChildren())
  {
    return ExecutionStatus::SUCCESS;
  }

  for (auto &wrapper : _wrappers)
  {
    auto wrapper_status = wrapper.GetStatus();

    if (NeedsExecute(wrapper_status))
    {
      wrapper.Tick(ui, ws);
    }
  }
  auto status = CalculateCompoundStatus();
  if (status != ExecutionStatus::RUNNING)
  {
    Halt();
  }
  return status;
}

void ParallelSequence::ResetHook()
{
  // call Halt when there are descendents running
  if (GetStatus() == ExecutionStatus::RUNNING)
  {
    Halt();
  }
  // wait for child threads to terminate
  _wrappers.clear();
  // call reset on child instructions
  ResetChildren();
}

bool ParallelSequence::SetupImpl(const Procedure &proc)
{
  bool status = SetupChildren(proc);
  if (status)
  {
    int N = static_cast<int>(ChildInstructions().size());

    // default values for thresholds:
    _success_th = N;
    _failure_th = 1;

    bool success_th_from_attributes = false;
    if (HasAttribute(SUCCESS_THRESHOLD_ATTRIBUTE))
    {
      auto success_th = GetAttribute(SUCCESS_THRESHOLD_ATTRIBUTE);
      try
      {
        _success_th = std::stoi(success_th);
        success_th_from_attributes = true;
      }
      catch (const std::exception &)
      {
        log::Warning(
            "ParallelSequence::InitThresholds() - could not parse successThreshold attribute!");
        status = false;
      }
    }

    if (HasAttribute(FAILURE_THRESHOLD_ATTRIBUTE))
    {
      auto failure_th = GetAttribute(FAILURE_THRESHOLD_ATTRIBUTE);
      try
      {
        int th = std::stoi(failure_th);
        if (success_th_from_attributes)
        {
          _failure_th = std::min(th, N - _success_th + 1);
        }
        else
        {
          _failure_th = th;
          _success_th = N - th + 1;
        }
      }
      catch (const std::exception &)
      {
        log::Warning(
            "ParallelSequence::InitThresholds() - could not parse failureThreshold attribute!");
        status = false;
      }
    }
  }
  return status;
}

ExecutionStatus ParallelSequence::CalculateCompoundStatus() const
{
  int n_success = 0;
  int n_failure = 0;

  for (const auto &wrapper : _wrappers)
  {
    auto wrapper_status = wrapper.GetStatus();

    if (wrapper_status == ExecutionStatus::SUCCESS)
    {
      n_success++;
    }
    if (wrapper_status == ExecutionStatus::FAILURE)
    {
      n_failure++;
    }
  }

  if (n_success >= _success_th)
  {
    return ExecutionStatus::SUCCESS;
  }
  if (n_failure >= _failure_th)
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::RUNNING;
}

void ParallelSequence::InitWrappers()
{
  _wrappers.clear();
  for (auto child : ChildInstructions())
  {
    _wrappers.emplace_back(child);
  }
}

ParallelSequence::ParallelSequence() : CompoundInstruction(Type), _success_th{0}, _failure_th{0} {}

ParallelSequence::~ParallelSequence() = default;

}  // namespace sequencer

}  // namespace sup
