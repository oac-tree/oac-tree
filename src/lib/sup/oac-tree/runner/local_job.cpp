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

#include <sup/oac-tree/job_info.h>
#include <sup/oac-tree/job_info_utils.h>
#include <sup/oac-tree/job_interface_adapter.h>
#include <sup/oac-tree/local_job.h>

namespace sup
{
namespace oac_tree
{
struct LocalJob::LocalJobImpl
{
  LocalJobImpl(std::unique_ptr<Procedure> proc, IJobInfoIO& job_info_io);
  ~LocalJobImpl() = default;

  std::unique_ptr<Procedure> m_proc;
  JobInterfaceAdapter m_job_interface;

  AsyncRunner m_runner;
  std::unique_ptr<JobInfo> m_job_info;
  std::vector<const oac_tree::Instruction*> m_ordered_instructions;
};

LocalJob::LocalJob(std::unique_ptr<Procedure> proc, IJobInfoIO& job_info_io)
  : IJob{}
  , m_impl{new LocalJobImpl{std::move(proc), job_info_io}}
{}

LocalJob::~LocalJob() = default;

LocalJob::LocalJob(LocalJob&& other)
  : IJob{}
  , m_impl{std::move(other.m_impl)}
{}

LocalJob& LocalJob::operator=(LocalJob&& other)
{
  LocalJob moved{std::move(other)};
  std::swap(m_impl, moved.m_impl);
  return *this;
}

const JobInfo& LocalJob::GetInfo() const
{
  return *m_impl->m_job_info;
}

void LocalJob::SetBreakpoint(sup::dto::uint32 instr_idx)
{
  if (instr_idx >= m_impl->m_ordered_instructions.size())
  {
    return;
  }
  auto instruction = m_impl->m_ordered_instructions[instr_idx];
  Runner().SetBreakpoint(instruction);
}

void LocalJob::RemoveBreakpoint(sup::dto::uint32 instr_idx)
{
  if (instr_idx >= m_impl->m_ordered_instructions.size())
  {
    return;
  }
  auto instruction = m_impl->m_ordered_instructions[instr_idx];
  Runner().RemoveBreakpoint(instruction);
}

void LocalJob::Start()
{
  Runner().Start();
}

void LocalJob::Step()
{
  Runner().Step();
}

void LocalJob::Pause()
{
  Runner().Pause();
}

void LocalJob::Reset()
{
  Runner().Reset();
}

void LocalJob::Halt()
{
  Runner().Halt();
}

AsyncRunner& LocalJob::Runner()
{
  return m_impl->m_runner;
}

LocalJob::LocalJobImpl::LocalJobImpl(std::unique_ptr<Procedure> proc, IJobInfoIO& job_info_io)
  : m_proc{std::move(proc)}
  , m_job_interface{*m_proc, job_info_io}
  , m_runner{*m_proc, m_job_interface}
  , m_job_info{}
  , m_ordered_instructions{}
{
  m_job_interface.InitializeInstructionTree(m_proc->RootInstruction());
  m_job_info.reset(
    new JobInfo{utils::CreateJobInfo(*m_proc, m_job_interface.GetInstructionMap())});
  m_ordered_instructions = m_job_interface.GetOrderedInstructions();
}

}  // namespace oac_tree

}  // namespace sup
