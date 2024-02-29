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

#ifndef SUP_SEQUENCER_APPLICATON_UTILS_H_
#define SUP_SEQUENCER_APPLICATON_UTILS_H_

#include <sup/sequencer/job_state_monitor.h>
#include <sup/sequencer/procedure.h>

#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>

namespace sup
{
namespace sequencer
{
namespace utils
{

/**
 * @brief Helper class to try to load a Procedure from file and call its Setup method.
 * On failure, it will contain the reason for failure as an error message.
 *
 */
class ProcedureLoader
{
public:
  explicit ProcedureLoader(const std::string& filename);
  ~ProcedureLoader();

  std::unique_ptr<Procedure> ParseAndSetup();

  std::string GetErrorMessage() const;
private:
  std::string m_filename;
  std::string m_error_message;
};

/**
 * @brief Simple implementation of JobStateMonitor that provides member functions that wait for
 * certain JobState conditions.
 *
 * @note This implementation ignores any updates on instruction breakpoints.
 *
 */
class SimpleJobStateMonitor : public JobStateMonitor
{
public:
  SimpleJobStateMonitor();
  ~SimpleJobStateMonitor();

  void OnStateChange(JobState state) noexcept override;

  void OnBreakpointChange(const Instruction* instruction, bool breakpoint_set) noexcept override;

  void OnProcedureTick(const Procedure& proc) noexcept override;

  JobState GetCurrentState() const;

  JobState WaitForFinished() const;

  bool WaitForState(JobState state, double seconds) const;
private:
  JobState m_state;
  mutable std::mutex m_mtx;
  mutable std::condition_variable m_cv;
};

}  // namespace utils

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_APPLICATON_UTILS_H_
