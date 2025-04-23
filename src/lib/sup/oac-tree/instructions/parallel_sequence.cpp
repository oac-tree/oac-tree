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

#include "parallel_sequence.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/generic_utils.h>

namespace sup
{
namespace oac_tree
{
const std::string ParallelSequence::Type = "ParallelSequence";

ParallelSequence::ParallelSequence()
  : CompoundInstruction(ParallelSequence::Type)
  , m_success_th{0}
  , m_failure_th{0}
{
  AddAttributeDefinition(Constants::SUCCESS_THRESHOLD_ATTRIBUTE_NAME,
                         sup::dto::UnsignedInteger32Type);
  AddAttributeDefinition(Constants::FAILURE_THRESHOLD_ATTRIBUTE_NAME,
                         sup::dto::UnsignedInteger32Type);
}

ParallelSequence::~ParallelSequence() = default;


bool ParallelSequence::InitHook(UserInterface& ui, Workspace& ws)
{
  InitWrappers();
  return InitThresholds(ui, ws);
}

ExecutionStatus ParallelSequence::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  for (auto &wrapper : m_wrappers)
  {
    auto wrapper_status = wrapper.GetStatus();
    if (NeedsExecute(wrapper_status))
    {
      wrapper.Tick(ui, ws);
    }
  }
  auto status = CalculateCompoundStatus();
  if (IsFinishedStatus(status))
  {
    Halt();
  }
  return status;
}

void ParallelSequence::ResetHook(UserInterface& ui)
{
  // call Halt when there are possibly descendents running
  auto status = GetStatus();
  if (status == ExecutionStatus::RUNNING || status == ExecutionStatus::NOT_FINISHED)
  {
    Halt();
  }
  // wait for child threads to terminate
  m_wrappers.clear();
  // call reset on child instructions
  ResetChildren(ui);
}

std::vector<const Instruction*> ParallelSequence::NextInstructionsImpl() const
{
  if (m_wrappers.empty())
  {
    return ChildInstructions();
  }
  std::vector<const Instruction*> result;
  for (auto& wrapper : m_wrappers)
  {
    if (ReadyForExecute(wrapper.GetStatus()))
    {
      result.push_back(wrapper.GetInstruction());
    }
  }
  return result;
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

bool ParallelSequence::InitThresholds(UserInterface& ui, Workspace& ws)
{
  auto N = static_cast<sup::dto::uint32>(ChildInstructions().size());
  m_success_th = N;
  // Literal attributes can't fail:
  if (!GetAttributeValueAs(Constants::SUCCESS_THRESHOLD_ATTRIBUTE_NAME, ws, ui, m_success_th))
  {
    return false;
  }
  m_success_th = std::min(N, m_success_th);
  m_failure_th = 1;
  if (!GetAttributeValueAs(Constants::FAILURE_THRESHOLD_ATTRIBUTE_NAME, ws, ui, m_failure_th))
  {
    return false;
  }
  m_failure_th = std::min(N, m_failure_th);
  if (HasAttribute(Constants::SUCCESS_THRESHOLD_ATTRIBUTE_NAME))
  {
    m_failure_th = std::min(m_failure_th, N - m_success_th + 1);
  }
  else if (HasAttribute(Constants::FAILURE_THRESHOLD_ATTRIBUTE_NAME))
  {
    m_success_th = N + 1 - m_failure_th;
  }
  return true;
}

}  // namespace oac_tree

}  // namespace sup
