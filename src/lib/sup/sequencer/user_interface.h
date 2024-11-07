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

#ifndef SUP_SEQUENCER_USER_INTERFACE_H_
#define SUP_SEQUENCER_USER_INTERFACE_H_

#include <sup/dto/anyvalue.h>

#include <string>
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
  kDefault = 0,      // Use default dialog type
  kSelection,        // Selection box (e.g. combo box)
  kConfirmation,     // Confirmation dialog (typically two buttons only)
  kAlert             // Alert dialog (typically one button only)
};
}

/**
 * @brief Interface representing user interactions.
 *
 * @details Implementers of concrete user interfaces should take care of thread safety, since many
 * of the member functions can be called from different threads.
 */
class UserInterface
{
public:

  virtual ~UserInterface();

  /**
   * @brief Method called when instruction's execution status changes.
   *
   * @param instruction Instruction that has new execution status.
   */
  virtual void UpdateInstructionStatus(const Instruction* instruction) = 0;

  /**
   * @brief Method called when a workspace Variable has received a value update.
   *
   * @param name Name of the Variable.
   * @param value New value of the Variable.
   * @param connected Boolean indicating if variable is connected.
   */
  virtual void VariableUpdated(const std::string& name, const sup::dto::AnyValue& value, bool connected) = 0;

  /**
   * @brief Method to put the value.
   *
   * @param value Value to put into UI.
   * @param description Optional description of the value.
   * @return true on successful retrieval of a value, false otherwise.
   */
  virtual bool PutValue(const sup::dto::AnyValue& value, const std::string& description) = 0;

  /**
   * @brief Method to request the user to input a value.
   *
   * @param value Value to be filled in.
   * @param description Optional description of the value.
   * @return true on successful retrieval of a value, false otherwise.
   */
  virtual bool GetUserValue(sup::dto::AnyValue& value, const std::string& description) = 0;

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
   */
  virtual int GetUserChoice(const std::vector<std::string>& options,
                            const sup::dto::AnyValue& metadata) = 0;

  /**
   * @brief Method called to display a message.
   * @param message Message to be displayed.
   *
   */
  virtual void Message(const std::string& message) = 0;

  /**
   * @brief Method called to log a message.
   *
   */
  virtual void Log(int severity, const std::string& message) = 0;
};

/**
 * @brief Convenience function to log an error to a UserInterface object.
 *
 * @param ui UserInterface that will handle the logging.
 * @param message Error message to log.
 */
void LogError(UserInterface& ui, const std::string& message);

/**
 * @brief Convenience function to log a warning to a UserInterface object.
 *
 * @param ui UserInterface that will handle the logging.
 * @param message Warning message to log.
 */
void LogWarning(UserInterface& ui, const std::string& message);

/**
 * @brief UserInterface with default implementations of virtual methods.
 *
 * @details The default implementations do as least as possible, i.e. nothing or returning a
 * status that indicates it could not perform the action.
 */
class DefaultUserInterface : public UserInterface
{
public:
  DefaultUserInterface();
  ~DefaultUserInterface();

  void UpdateInstructionStatus(const Instruction* instruction) override;
  void VariableUpdated(const std::string& name, const sup::dto::AnyValue& value,
                       bool connected) override;
  bool PutValue(const sup::dto::AnyValue& value, const std::string& description) override;
  bool GetUserValue(sup::dto::AnyValue& value, const std::string& description) override;
  int GetUserChoice(const std::vector<std::string>& options,
                    const sup::dto::AnyValue& metadata) override;
  void Message(const std::string& message) override;
  void Log(int severity, const std::string& message) override;
};

sup::dto::AnyValue CreateUserChoiceMetadata();

bool IsUserChoiceMetadata(const sup::dto::AnyValue& metadata);

std::string GetMainTextFromMetadata(const sup::dto::AnyValue& metadata);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_USER_INTERFACE_H_
