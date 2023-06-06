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
  , m_success_th{0}
  , m_failure_th{0}
{
  AddAttributeDefinition(SUCCESS_THRESHOLD_ATTRIBUTE, sup::dto::UnsignedInteger32Type);
  AddAttributeDefinition(FAILURE_THRESHOLD_ATTRIBUTE, sup::dto::UnsignedInteger32Type);
}

ParallelSequence::~ParallelSequence() = default;


void ParallelSequence::InitHook()
{
  InitWrappers();
}

void ParallelSequence::SetupImpl(const Procedure &proc)
{
  SetupChildren(proc);

  // default values for thresholds:
  auto N = static_cast<sup::dto::uint32>(ChildInstructions().size());
  m_success_th = N;
  m_failure_th = 1;
  bool success_th_from_attributes = false;
  if (HasAttribute(SUCCESS_THRESHOLD_ATTRIBUTE))
  {
    m_success_th = std::min(N, GetAttributeValue<sup::dto::uint32>(SUCCESS_THRESHOLD_ATTRIBUTE));
    success_th_from_attributes = true;
  }
  if (HasAttribute(FAILURE_THRESHOLD_ATTRIBUTE))
  {
    auto th = std::min(N, GetAttributeValue<sup::dto::uint32>(FAILURE_THRESHOLD_ATTRIBUTE));
    if (success_th_from_attributes)
    {
      m_failure_th = std::min(th, N - m_success_th + 1);
    }
    else
    {
      m_failure_th = th;
      m_success_th = N - th + 1;
    }
  }
}

ExecutionStatus ParallelSequence::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (!HasChildren())
  {
    return ExecutionStatus::SUCCESS;
  }
  for (auto &wrapper : m_wrappers)
  {
    auto wrapper_status = wrapper.GetStatus();
    if (NeedsExecute(wrapper_status) && wrapper_status != ExecutionStatus::RUNNING)
    {
      wrapper.Tick(ui, ws);
    }
    else
    {
      wrapper.UpdateStatus();
    }
  }
  auto status = CalculateCompoundStatus();
  if (IsFinishedStatus(status))
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
  m_wrappers.clear();
  // call reset on child instructions
  ResetChildren();
}

ExecutionStatus ParallelSequence::CalculateCompoundStatus() const
{
  int n_success = 0;
  int n_failure = 0;
  bool child_not_finished_present = false;
  for (const auto &wrapper : m_wrappers)
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
    if (wrapper_status == ExecutionStatus::NOT_FINISHED)
    {
      child_not_finished_present = true;
    }
  }
  if (n_success >= m_success_th)
  {
    return ExecutionStatus::SUCCESS;
  }
  if (n_failure >= m_failure_th)
  {
    return ExecutionStatus::FAILURE;
  }
  return child_not_finished_present ? ExecutionStatus::NOT_FINISHED : ExecutionStatus::RUNNING;
}

void ParallelSequence::InitWrappers()
{
  m_wrappers.clear();
  for (auto child : ChildInstructions())
  {
    m_wrappers.emplace_back(child);
  }
}

}  // namespace sequencer

}  // namespace sup
