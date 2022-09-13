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
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_USER_INTERFACE_H_
#define SUP_SEQUENCER_USER_INTERFACE_H_

#include <common/AnyValue.h>

#include <mutex>
#include <string>
#include <vector>

namespace sup
{
namespace sequencer
{
class Instruction;

/**
 * @brief Interface representing user interactions.
 */
class UserInterface
{
private:
  /**
   * @brief Private pure virtual implementation of
   * UserInterface::UpdateInstructionStatus(const Instruction * instruction).
   */
  virtual void UpdateInstructionStatusImpl(const Instruction* instruction) = 0;

  /**
   * @brief Private virtual implementation of
   * UserInterface::VariableUpdated(const std::string& name, const ccs::types::AnyValue& value).
   */
  virtual void VariableUpdatedImpl(const std::string& name, const ccs::types::AnyValue& value);

  /**
   * @brief Private virtual implementation of
   * UserInterface::PutValue(const AnyValue & value, const std::string & description).
   *
   * @note Default implementation returns false.
   */
  virtual bool PutValueImpl(const ::ccs::types::AnyValue& value, const std::string& description);

  /**
   * @brief Private virtual implementation of
   * UserInterface::GetUserValue(AnyValue & value, const std::string & description).
   *
   * @note Default implementation returns false.
   */
  virtual bool GetUserValueImpl(::ccs::types::AnyValue& value, const std::string& description);

  /**
   * @brief Private virtual implementation of
   * UserInterface::GetUserChoice(const std::vector<std::string> & choices, const std::string &
   * description).
   *
   * @note Default implementation returns -1.
   */
  virtual int GetUserChoiceImpl(const std::vector<std::string>& choices,
                                const std::string& description);

  /**
   * @brief Private virtual implementation of
   * UserInterface::StartSingleStep().
   *
   * @note Default implementation is empty.
   */
  virtual void StartSingleStepImpl();

  /**
   * @brief Private virtual implementation of
   * UserInterface::EndSingleStep().
   *
   * @note Default implementation is empty.
   */
  virtual void EndSingleStepImpl();

  /**
   * @brief Private virtual implementation of
   * UserInterface::Message().
   *
   * @note Default implementation is empty.
   */
  virtual void MessageImpl(const std::string& message);

  /**
   * @brief Mutex for concurrent access of UserInterface.
   */
  mutable std::mutex _ui_mutex;

protected:
public:
  /**
   * @brief Constructor.
   */
  UserInterface() = default;

  /**
   * @brief Destructor.
   */
  virtual ~UserInterface() = default;

  /**
   * @brief Method called when instruction's execution status changes.
   *
   * @param instruction Instruction that has new execution status.
   * @note Non-Virtual Interface.
   */
  void UpdateInstructionStatus(const Instruction* instruction);

  /**
   * @brief Method called when a workspace Variable has received a value update.
   *
   * @param name Name of the Variable.
   * @param value New value of the Variable.
   * @note Non-Virtual Interface.
   */
  void VariableUpdated(const std::string& name, const ccs::types::AnyValue& value);

  /**
   * @brief Method to put the value.
   *
   * @param value Value to put into UI.
   * @param description Optional description of the value.
   * @return true on successful retrieval of a value, false otherwise.
   * @note Non-Virtual Interface.
   */
  bool PutValue(const ::ccs::types::AnyValue& value, const std::string& description = {});

  /**
   * @brief Method to request the user to input a value.
   *
   * @param value Value to be filled in.
   * @param description Optional description of the value.
   * @return true on successful retrieval of a value, false otherwise.
   * @note Non-Virtual Interface.
   */
  bool GetUserValue(::ccs::types::AnyValue& value, const std::string& description = {});

  /**
   * @brief Method to request the user to choose one of the given options.
   *
   * @param choices List of options to choose from.
   * @param description Optional description.
   * @return index of the choice (zero-based) or -1 on failure.
   * @note Non-Virtual Interface.
   */
  int GetUserChoice(const std::vector<std::string>& choices, const std::string& description = {});

  /**
   * @brief Method called before stepping into an instruction.
   *
   * @note Non-Virtual Interface.
   */
  void StartSingleStep();

  /**
   * @brief Method called after executing an instruction.
   *
   * @note Non-Virtual Interface.
   */
  void EndSingleStep();

  /**
   * @brief Method called to display a message.
   *
   * @note Non-Virtual Interface.
   */
  void Message(const std::string& message);
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_USER_INTERFACE_H_
