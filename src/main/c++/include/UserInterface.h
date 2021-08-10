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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

/**
 * @file UserInterface.h
 * @brief Header file for UserInterface class.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the UserInterface class.
 */

#ifndef _SEQ_UserInterface_h_
#define _SEQ_UserInterface_h_

// Global header files

#include <common/AnyValue.h>

#include <mutex>
#include <string>
#include <vector>

// Local header files

// Constants

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
// Forward declarations

class Instruction;

// Type definition

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
};

// Global variables

// Function declarations

// Function definitions

}  // namespace sequencer

}  // namespace sup

extern "C"
{
#endif  // __cplusplus

  // C API function declarations

#ifdef __cplusplus
}  // extern C
#endif  // __cplusplus

#endif  // _SEQ_UserInterface_h_
