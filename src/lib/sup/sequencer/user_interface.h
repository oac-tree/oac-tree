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

#ifndef SUP_SEQUENCER_USER_INTERFACE_H_
#define SUP_SEQUENCER_USER_INTERFACE_H_

#include <sup/dto/anyvalue.h>

#include <mutex>
#include <string>
#include <utility>
#include <vector>

namespace sup
{
namespace sequencer
{
class Instruction;

namespace dialog_type
{
/**
 * @brief DialogType enumerators provide a hint to a UserInterface implementation on how to
 * represent certain user choices. Implementations are allowed to ignore these hints.
 */
enum DialogType
{
  kDefault = 0,      // Use default dialog tyep
  kSelection,        // Selection box (e.g. combo box)
  kConfirmation,     // Confirmation dialog (typically two buttons only)
  kAlert             // Alert dialog (typically one button only)
};
}

/**
 * @brief Interface representing user interactions.
 */
class UserInterface
{
public:
  UserInterface();

  virtual ~UserInterface();

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
   * @param connected Boolean indicating if variable is connected.
   * @note Non-Virtual Interface.
   */
  void VariableUpdated(const std::string& name, const sup::dto::AnyValue& value, bool connected);

  /**
   * @brief Method to put the value.
   *
   * @param value Value to put into UI.
   * @param description Optional description of the value.
   * @return true on successful retrieval of a value, false otherwise.
   * @note Non-Virtual Interface.
   */
  bool PutValue(const sup::dto::AnyValue& value, const std::string& description = {});

  /**
   * @brief Method to request the user to input a value.
   *
   * @param value Value to be filled in.
   * @param description Optional description of the value.
   * @return true on successful retrieval of a value, false otherwise.
   * @note Non-Virtual Interface.
   */
  bool GetUserValue(sup::dto::AnyValue& value, const std::string& description = {});

  /**
   * @brief Method to request the user to choose one of the given options.
   *
   * @param options List of options the user can choose; each option is a string.
   * @param metadata Structure that encodes optional metadata that could be used by the
   *                 UserInterface to properly display the choice.
   *
   * @return value of the choice.
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
   * @note Non-Virtual Interface.
   */
  int GetUserChoice(const std::vector<std::string>& options,
                    const sup::dto::AnyValue& metadata = {});

  /**
   * @brief Method called to display a message.
   *
   * @note Non-Virtual Interface.
   */
  void Message(const std::string& message);

  /**
   * @brief Method called to log a message.
   *
   * @note Non-Virtual Interface.
   */
  void Log(int severity, const std::string& message);

  /**
   * @brief Convenience method to log an error message.
   */
  void LogError(const std::string& message);

  /**
   * @brief Convenience method to log a warning message.
   */
  void LogWarning(const std::string& message);

private:
  /**
   * @brief Private pure virtual implementation of
   * UserInterface::UpdateInstructionStatus(const Instruction * instruction).
   */
  virtual void UpdateInstructionStatusImpl(const Instruction* instruction) = 0;

  /**
   * @brief Private virtual implementation of
   * UserInterface::VariableUpdated(const std::string& name, const sup::dto::AnyValue& value).
   */
  virtual void VariableUpdatedImpl(const std::string& name, const sup::dto::AnyValue& value,
                                   bool connected);

  /**
   * @brief Private virtual implementation of
   * UserInterface::PutValue(const AnyValue & value, const std::string & description).
   *
   * @note Default implementation returns false.
   */
  virtual bool PutValueImpl(const sup::dto::AnyValue& value, const std::string& description);

  /**
   * @brief Private virtual implementation of
   * UserInterface::GetUserValue(AnyValue & value, const std::string & description).
   *
   * @note Default implementation returns false.
   */
  virtual bool GetUserValueImpl(sup::dto::AnyValue& value, const std::string& description);

  /**
   * @brief Private virtual implementation of
   * UserInterface::GetUserChoice(const std::vector<std::string>& options,
   *                              const sup::dto::AnyValue& metadata)
   *
   * @note Default implementation returns -1.
   */
  virtual int GetUserChoiceImpl(const std::vector<std::string>& options,
                                const sup::dto::AnyValue& metadata);

  /**
   * @brief Private virtual implementation of UserInterface::Message().
   *
   * @note Default implementation is empty.
   */
  virtual void MessageImpl(const std::string& message);

  /**
   * @brief Private virtual implementation of UserInterface::Log().
   *
   * @note Default implementation is empty.
   */
  virtual void LogImpl(int severity, const std::string& message);

  /**
   * @brief Mutex for concurrent access of UserInterface.
   */
  mutable std::mutex m_ui_mutex;
};

sup::dto::AnyValue CreateUserChoiceMetadata();

bool IsUserChoiceMetadata(const sup::dto::AnyValue& metadata);

std::string GetMainTextFromMetadata(const sup::dto::AnyValue& metadata);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_USER_INTERFACE_H_
