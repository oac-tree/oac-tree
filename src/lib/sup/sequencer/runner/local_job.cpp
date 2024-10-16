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
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/sequencer/local_job.h>

namespace sup
{
namespace sequencer
{
// TODO: remove these temporary stubs when the real classes are present
class JobInfo {};

class TestJobInterface : public JobInterface
{
public:
  TestJobInterface() = default;
  ~TestJobInterface() = default;
  void OnStateChange(JobState) noexcept override {}
  void OnBreakpointChange(const Instruction*, bool) noexcept override {}
  void OnProcedureTick(const Procedure&) noexcept override {}

  void UpdateInstructionStatus(const Instruction*) override {}
  void VariableUpdated(const std::string&, const sup::dto::AnyValue&, bool) override {}
  bool PutValue(const sup::dto::AnyValue&, const std::string&) override { return false; }
  bool GetUserValue(sup::dto::AnyValue&, const std::string&) override { return false; }
  int GetUserChoice(const std::vector<std::string>&, const sup::dto::AnyValue&) override {
    return -1;
  }
  void Message(const std::string& message) override {}
  void Log(int severity, const std::string& message) override {}
};

struct LocalJob::LocalJobImpl
{
  LocalJobImpl(const std::string& prefix, std::unique_ptr<sup::sequencer::Procedure> proc,
               IJobInfoIO& job_info_io);
  ~LocalJobImpl() = default;

  std::unique_ptr<sup::sequencer::Procedure> m_proc;
  TestJobInterface m_job_interface;
  sup::sequencer::AsyncRunner m_runner;
  JobInfo m_job_info;
};

LocalJob::LocalJob(const std::string& prefix, std::unique_ptr<sup::sequencer::Procedure> proc,
                   IJobInfoIO& job_info_io)
  : IJob{}
  , m_impl{new LocalJobImpl{prefix, std::move(proc), job_info_io}}
{}

LocalJob::~LocalJob() = default;

LocalJob::LocalJob(LocalJob&& other)
  : IJob{}
  , m_impl{std::move(other.m_impl)}
{}

LocalJob& LocalJob::operator=(LocalJob&& other)
{
  std::swap(m_impl, other.m_impl);
  return *this;
}

const JobInfo& LocalJob::GetInfo() const
{
  return m_impl->m_job_info;
}

void LocalJob::SetBreakpoint(sup::dto::uint32 instr_idx)
{
  // TODO: use real pointer
  Runner().SetBreakpoint(nullptr);
}

void LocalJob::RemoveBreakpoint(sup::dto::uint32 instr_idx)
{
  // TODO: use real pointer
  Runner().RemoveBreakpoint(nullptr);
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

sup::sequencer::AsyncRunner& LocalJob::Runner()
{
  return m_impl->m_runner;
}

LocalJob::LocalJobImpl::LocalJobImpl(
  const std::string& prefix, std::unique_ptr<sup::sequencer::Procedure> proc,
  IJobInfoIO& job_info_io)
  : m_proc{std::move(proc)}
  , m_job_interface{}
  , m_runner{*m_proc, m_job_interface}
  , m_job_info{}
{}

}  // namespace sequencer

}  // namespace sup
