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

  /**
   * @brief Initialize the object with the number of instructions. This is typically done after
   * construction when the instruction tree is correctly setup.
   *
   * @param n_instr Number of instructions in the tree.
   */
  virtual void InitNumberOfInstructions(sup::dto::uint32 n_instr) = 0;

  /**
   * @brief Notify an update of an instruction's state, i.e. its execution status and breakpoint
   * information.
   *
   * @param instr_idx Index of the instruction.
   * @param state Updated state of the instruction.
   */
  virtual void InstructionStateUpdated(sup::dto::uint32 instr_idx, InstructionState state) = 0;

  /**
   * @brief Notify an update of a variable's state.
   *
   * @param var_idx Index of the variable.
   * @param value Updated value of the variable.
   * @param connected Updated connected status of the variable.
   */
  virtual void VariableUpdated(sup::dto::uint32 var_idx, const sup::dto::AnyValue& value,
                               bool connected) = 0;

  /**
   * @brief Notify an update in the job's state.
   *
   * @param state Update job state.
   */
  virtual void JobStateUpdated(JobState state) = 0;

  /**
   * @brief Output a value to the user.
   *
   * @param value Value to output.
   * @param description Description of the value that is output.
   */
  virtual void PutValue(const sup::dto::AnyValue& value, const std::string& description) = 0;

  /**
   * @brief Get a user provided AnyValue. Note that since user input is typically a blocking
   * operation, there is also an Interrupt member function that can be used to interrupt a given
   * input function.
   *
   * @param id Identification of this user input request (to match with possible Interrupt calls)
   * @param value Output parameter for the user provided value.
   * @param description Description of the value the user needs to provide.
   * @return true on success, false otherwise (e.g. when interrupted)
   */
  virtual bool GetUserValue(sup::dto::uint64 id, sup::dto::AnyValue& value,
                            const std::string& description) = 0;

  /**
   * @brief Get a user provided choice as an integer. Note that since user input is typically a
   * blocking operation, there is also an Interrupt member function that can be used to interrupt a
   * given input function.
   *
   * @param id Identification of this user input request (to match with possible Interrupt calls)
   * @param options List of options the user can choose between.
   * @param metadata Metadata for the user interface to distinguish different formatting options.
   * @return true on success, false otherwise (e.g. when interrupted)
   *
   * @details The provided metadata has the following structure (which may be extended in the
   * future):
   * sup::sequencerUserChoiceMetadata/v1.0
   *   string text (opt)                  Main text to display
   *   uint dialog_type (opt)             Enumerator giving the type of dialog: e.g. confirmation
   *                                      dialog, combobox style dialog, etc.
   *   bool modal (opt)                   Flag to indicate preference for (non)modal dialog
   *   string title (opt)                 Title for the dialog
   *   string informative (opt)           Informative extra text to display
   *   string details (opt)               Details about the user choice, which could be displayed
   *                                      on request
   *   ...                                Future extension appear here
   */
  virtual int GetUserChoice(sup::dto::uint64 id, const std::vector<std::string>& options,
                            const sup::dto::AnyValue& metadata) = 0;

  /**
   * @brief Interrupt a user input request.
   *
   * @note Due to multithreading, an interrupt request with a specific id may be triggered before
   * the corresponding input method was called. Implementations need to take care of this case.
   *
   * @param id Identification of the user input request to interrupt. Implementations may choose
   * to do nothing in such interrupt requests. In that case, a user interaction will be needed to
   * properly cleanup the underlying user interface, since it will join the threads that are
   * spawned to get user input.
   */
  virtual void Interrupt(sup::dto::uint64 id) = 0;

  /**
   * @brief Ouput a message to the user.
   *
   * @param message Message to output.
   */
  virtual void Message(const std::string& message) = 0;

  /**
   * @brief Log a message.
   *
   * @param severity Severity of the log message.
   * @param message Message to log.
   */
  virtual void Log(int severity, const std::string& message) = 0;

  /**
   * @brief Notify a change in the set of instructions that will be ticked next.
   *
   * @param instr_indices List of instruction indices.
   */
  virtual void NextInstructionsUpdated(const std::vector<sup::dto::uint32>& instr_indices) = 0;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_I_JOB_INFO_IO_H_
