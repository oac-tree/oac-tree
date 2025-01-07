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

#ifndef SUP_SEQUENCER_JOB_INTERFACE_ADAPTER_H_
#define SUP_SEQUENCER_JOB_INTERFACE_ADAPTER_H_

#include <sup/sequencer/async_input_adapter.h>
#include <sup/sequencer/i_job_info_io.h>
#include <sup/sequencer/instruction_state.h>
#include <sup/sequencer/job_interface.h>
#include <sup/sequencer/job_map.h>

#include <vector>

namespace sup
{
namespace sequencer
{

class JobInterfaceAdapter : public JobInterface
{
public:
  JobInterfaceAdapter(const Procedure& proc, IJobInfoIO& job_info_io);

  ~JobInterfaceAdapter();

  void InitializeInstructionTree(const Instruction* root);

  const InstructionMap& GetInstructionMap() const;

  std::vector<const Instruction*> GetOrderedInstructions() const;

  void UpdateInstructionStatus(const Instruction* instruction) override;
  void VariableUpdated(const std::string& name, const sup::dto::AnyValue& value,
                       bool connected) override;
  bool PutValue(const sup::dto::AnyValue& value, const std::string& description) override;
  std::unique_ptr<IUserInputFuture> RequestUserInput(const UserInputRequest& request) override;
  void Message(const std::string& message) override;
  void Log(int severity, const std::string& message) override;

  void OnStateChange(JobState state) noexcept override;
  void OnBreakpointChange(const Instruction* instruction,
                          bool breakpoint_set) noexcept override;
  void OnProcedureTick(const Procedure& proc) override;

private:
  JobMap m_job_map;
  std::vector<InstructionState> m_instr_states;
  IJobInfoIO& m_job_info_io;
  AsyncInputAdapter m_input_adapter;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_JOB_INTERFACE_ADAPTER_H_
