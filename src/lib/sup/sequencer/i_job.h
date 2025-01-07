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

#ifndef SUP_SEQUENCER_I_JOB_H_
#define SUP_SEQUENCER_I_JOB_H_

#include <sup/dto/basic_scalar_types.h>

namespace sup
{
namespace sequencer
{
class JobInfo;

/**
 * @brief Interface for managing and controlling a single job. Concrete implementations may
 * represent locally executing jobs, remotely executing jobs, etc.
*/
class IJob
{
public:
  IJob() = default;
  virtual ~IJob();

  IJob(const IJob&) = delete;
  IJob& operator=(const IJob&) = delete;
  IJob(IJob&&) = delete;
  IJob& operator=(IJob&&) = delete;

  /**
   * @brief Get all the static information about the managed job: procedure name, variable types,
   * intruction tree, etc.
   *
   * @return JobInfo reference.
   */
  virtual const JobInfo& GetInfo() const = 0;

   /**
   * @brief Set a breakpoint at the given instruction.
   *
   * @param instr_idx Index of the instruction for which to set a breakpoint.
   */
  virtual void SetBreakpoint(sup::dto::uint32 instr_idx) = 0;

  /**
   * @brief Remove a breakpoint at the given instruction.
   *
   * @param instr_idx Index of the instruction for which to set a breakpoint.
   */
  virtual void RemoveBreakpoint(sup::dto::uint32 instr_idx) = 0;

 /**
   * @brief Run the job in a continuous manner if allowed.
   */
  virtual void Start() = 0;

  /**
   * @brief Execute a single step of the job if allowed.
   */
  virtual void Step() = 0;

  /**
   * @brief Pause the job's execution.
   */
  virtual void Pause() = 0;

  /**
   * @brief Reset the job to its initial state.
   */
  virtual void Reset() = 0;

  /**
   * @brief Halt the job. This prevents continuing it afterwards. The job has to be
   * reset to be able to run again.
   */
  virtual void Halt() = 0;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_I_JOB_H_
