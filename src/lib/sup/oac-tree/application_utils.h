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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_APPLICATON_UTILS_H_
#define SUP_OAC_TREE_APPLICATON_UTILS_H_

#include <sup/oac-tree/async_runner.h>
#include <sup/oac-tree/job_state_monitor.h>
#include <sup/oac-tree/procedure.h>

#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>

namespace sup
{
namespace oac_tree
{
namespace utils
{

/**
 * @brief Helper function to parse a procedure from file. It catches exceptions and puts the
 * exception (or other error) message in the output parameter.
 */
std::unique_ptr<Procedure> SafeParseProcedure(const std::string& filename, std::string& error_msg);

/**
 * @brief Helper function to create a AsyncRunner. It catches exceptions and puts the exception
 * message in the output parameter.
 */
std::unique_ptr<AsyncRunner> CreateAsyncRunner(Procedure& proc, UserInterface& ui,
                                                   JobStateMonitor& state_monitor,
                                                   std::string& error_msg);

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

  void OnProcedureTick(const Procedure& proc) override;

  JobState GetCurrentState() const;

  JobState WaitForFinished() const;

  bool WaitForState(JobState state, double seconds) const;
private:
  JobState m_state;
  mutable std::mutex m_mtx;
  mutable std::condition_variable m_cv;
};

}  // namespace utils

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_APPLICATON_UTILS_H_
