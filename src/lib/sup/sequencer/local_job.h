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

#ifndef SUP_SEQUENCER_LOCAL_JOB_H_
#define SUP_SEQUENCER_LOCAL_JOB_H_

#include <sup/sequencer/async_runner.h>
#include <sup/sequencer/i_job.h>
#include <sup/sequencer/procedure.h>

#include <memory>
#include <string>

namespace sup
{
namespace sequencer
{
class IJobInfoIO;

/**
 * @brief Implementation of IJob that manages a locally running job.
*/
class LocalJob : public IJob
{
public:
  /**
   * @brief Construct locally running job from the given procedure.
   *
   * @param prefix Identifier for this specific job. Will be encoded in the JobInfo object.
   * @param proc Procedure to be managed by this job.
   * @param job_info_io IJobInfoIO implementation that will receive all updates and provide user
   * input/output.
   */
  LocalJob(const std::string& prefix, std::unique_ptr<Procedure> proc, IJobInfoIO& job_info_io);
  ~LocalJob();

  // Delete copy operations
  LocalJob(const LocalJob& other) = delete;
  LocalJob& operator=(const LocalJob& other) = delete;

  // Support move operations
  LocalJob(LocalJob&& other);
  LocalJob& operator=(LocalJob&& other);

  const JobInfo& GetInfo() const override;

  void SetBreakpoint(sup::dto::uint32 instr_idx) override;
  void RemoveBreakpoint(sup::dto::uint32 instr_idx) override;

  void Start() override;
  void Step() override;
  void Pause() override;
  void Reset() override;
  void Halt() override;

private:
  AsyncRunner& Runner();
  struct LocalJobImpl;
  std::unique_ptr<LocalJobImpl> m_impl;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_LOCAL_JOB_H_
