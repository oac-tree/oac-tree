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

#ifndef SUP_SEQUENCER_I_JOB_INFO_IO_H_
#define SUP_SEQUENCER_I_JOB_INFO_IO_H_

#include <sup/sequencer/instruction_state.h>
#include <sup/sequencer/job_states.h>

#include <sup/dto/anyvalue.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Abstract interface for runtime updates and user input/output of a job, represented by
 * a JobInfo.
 */
class IJobInfoIO
{
public:
  virtual ~IJobInfoIO();

  virtual void InitNumberOfInstructions(sup::dto::uint32 n_instr) = 0;

  virtual void InstructionStateUpdated(sup::dto::uint32 instr_idx, InstructionState state) = 0;

  virtual void VariableUpdated(sup::dto::uint32 var_idx, const sup::dto::AnyValue& value,
                               bool connected) = 0;

  virtual void JobStateUpdated(JobState state) = 0;

  virtual void PutValue(const sup::dto::AnyValue& value, const std::string& description) = 0;

  virtual bool GetUserValue(sup::dto::AnyValue& value, const std::string& description) = 0;

  virtual int GetUserChoice(const std::vector<std::string>& options,
                            const sup::dto::AnyValue& metadata) = 0;

  virtual void Message(const std::string& message) = 0;

  virtual void Log(int severity, const std::string& message) = 0;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_I_JOB_INFO_IO_H_
