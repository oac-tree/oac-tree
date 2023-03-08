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

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/generic_utils.h>

namespace sup
{
namespace sequencer
{
const std::string ParallelSequence::Type = "ParallelSequence";

const std::string SUCCESS_THRESHOLD_ATTRIBUTE = "successThreshold";
const std::string FAILURE_THRESHOLD_ATTRIBUTE = "failureThreshold";

ParallelSequence::ParallelSequence()
  : CompoundInstruction(ParallelSequence::Type)
  , _success_th{0}
  , _failure_th{0}
{}

ParallelSequence::~ParallelSequence() = default;


void ParallelSequence::InitHook()
{
  InitWrappers();
}

void ParallelSequence::SetupImpl(const Procedure &proc)
{
  SetupChildren(proc);

  // default values for thresholds:
  int N = static_cast<int>(ChildInstructions().size());
  _success_th = N;
  _failure_th = 1;
  bool success_th_from_attributes = false;
  if (HasAttribute(SUCCESS_THRESHOLD_ATTRIBUTE))
  {
    if (!utils::SafeStringToInt(_success_th, GetAttribute(SUCCESS_THRESHOLD_ATTRIBUTE)))
    {
      std::string error_message = InstructionSetupExceptionProlog(*this) +
        "could not parse value of attribute with name [" + SUCCESS_THRESHOLD_ATTRIBUTE + "]";
      throw InstructionSetupException(error_message);
    }
    success_th_from_attributes = true;
  }

  if (HasAttribute(FAILURE_THRESHOLD_ATTRIBUTE))
  {
    int th{};
    if (!utils::SafeStringToInt(th, GetAttribute(FAILURE_THRESHOLD_ATTRIBUTE)))
    {
      std::string error_message = InstructionSetupExceptionProlog(*this) +
        "could not parse value of attribute with name [" + FAILURE_THRESHOLD_ATTRIBUTE + "]";
      throw InstructionSetupException(error_message);
    }
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

}  // namespace sequencer

}  // namespace sup
