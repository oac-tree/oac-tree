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

#include <sup/oac-tree/job_interface_adapter.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/procedure.h>

namespace
{
using namespace sup::oac_tree;
UserInputReply ForwardUserInput(IJobInfoIO& job_info_io, const UserInputRequest& request,
                                sup::dto::uint64 id);
void ForwardInterrupt(IJobInfoIO& job_info_io, sup::dto::uint64 id);
}  // unnamed namespace

namespace sup
{
namespace oac_tree
{
using namespace std::placeholders;

JobInterfaceAdapter::JobInterfaceAdapter(const Procedure& proc, IJobInfoIO& job_info_io)
  : m_job_map{proc}
  , m_instr_states{}
  , m_job_info_io{job_info_io}
  , m_input_adapter{std::bind(&ForwardUserInput, std::ref(m_job_info_io), _1, _2),
                    std::bind(&ForwardInterrupt, std::ref(m_job_info_io), _1)}
{}

JobInterfaceAdapter::~JobInterfaceAdapter() = default;

void JobInterfaceAdapter::InitializeInstructionTree(const Instruction* root)
{
  m_job_map.InitializeInstructionTree(root);
  auto n_instr = m_job_map.GetNumberOfInstructions();
  m_job_info_io.InitNumberOfInstructions(n_instr);
  InstructionState initial_state{ false, ExecutionStatus::NOT_STARTED };
  m_instr_states = std::vector<InstructionState>(n_instr, initial_state);
}

const InstructionMap& JobInterfaceAdapter::GetInstructionMap() const
{
  return m_job_map.GetInstructionMap();
}

std::vector<const Instruction*> JobInterfaceAdapter::GetOrderedInstructions() const
{
  return m_job_map.GetOrderedInstructions();
}

void JobInterfaceAdapter::UpdateInstructionStatus(const Instruction* instruction)
{
  auto instr_idx = m_job_map.GetInstructionIndex(instruction);
  auto status = instruction->GetStatus();
  auto& instr_state = m_instr_states[instr_idx];
  instr_state.m_execution_status = status;
  m_job_info_io.InstructionStateUpdated(instr_idx, instr_state);
}

void JobInterfaceAdapter::VariableUpdated(const std::string& name, const sup::dto::AnyValue& value,
                                          bool connected)
{
  auto var_idx = m_job_map.GetVariableIndex(name);
  m_job_info_io.VariableUpdated(var_idx, value, connected);
}

bool JobInterfaceAdapter::PutValue(const sup::dto::AnyValue& value, const std::string& description)
{
  m_job_info_io.PutValue(value, description);
  return true;
}

std::unique_ptr<IUserInputFuture> JobInterfaceAdapter::RequestUserInput(
  const UserInputRequest& request)
{
  return m_input_adapter.AddUserInputRequest(request);
}

void JobInterfaceAdapter::Message(const std::string& message)
{
  return m_job_info_io.Message(message);
}

void JobInterfaceAdapter::Log(int severity, const std::string& message)
{
  return m_job_info_io.Log(severity, message);
}

void JobInterfaceAdapter::OnStateChange(JobState state) noexcept
{
  m_job_info_io.JobStateUpdated(state);
}

void JobInterfaceAdapter::OnBreakpointChange(const Instruction* instruction,
                                             bool breakpoint_set) noexcept
{
  auto instr_idx = m_job_map.GetInstructionIndex(instruction);
  auto& instr_state = m_instr_states[instr_idx];
  instr_state.m_breakpoint_set = breakpoint_set;
  m_job_info_io.InstructionStateUpdated(instr_idx, instr_state);
}

void JobInterfaceAdapter::OnProcedureTick(const Procedure& proc)
{
  std::vector<sup::dto::uint32> next_instr_indices{};
  auto next_instructions = GetNextLeaves(proc);
  for (const auto* instr : next_instructions)
  {
    next_instr_indices.push_back(m_job_map.GetInstructionIndex(instr));
  }
  m_job_info_io.NextInstructionsUpdated(next_instr_indices);
}

}  // namespace oac_tree

}  // namespace sup

namespace
{
UserInputReply ForwardUserInput(IJobInfoIO& job_info_io, const UserInputRequest& request,
                                sup::dto::uint64 id)
{
  switch (request.m_request_type)
  {
  case InputRequestType::kUserValue:
  {
    sup::dto::AnyValue value{};
    std::string description{};
    if (!ParseUserValueRequest(request, value, description))
    {
      return kInvalidUserInputReply;
    }
    if (!job_info_io.GetUserValue(id, value, description))
    {
      return CreateUserValueReply(false, {});
    }
    return CreateUserValueReply(true, value);
  }
  case InputRequestType::kUserChoice:
  {
    std::vector<std::string> options{};
    sup::dto::AnyValue metadata{};
    if (!ParseUserChoiceRequest(request, options, metadata))
    {
      return kInvalidUserInputReply;
    }
    auto choice = job_info_io.GetUserChoice(id, options, metadata);
    if (choice < 0)
    {
      return CreateUserChoiceReply(false, -1);
    }
    return CreateUserChoiceReply(true, choice);
  }
  default:
    break;
  }
  return kInvalidUserInputReply;
}

void ForwardInterrupt(IJobInfoIO& job_info_io, sup::dto::uint64 id)
{
  job_info_io.Interrupt(id);
}

}  // unnamed namespace

