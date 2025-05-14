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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_USER_INTERFACE_H_
#define SUP_OAC_TREE_USER_INTERFACE_H_

#include <sup/oac-tree/i_user_input_future.h>
#include <sup/oac-tree/user_input_request.h>

#include <sup/dto/anyvalue.h>

#include <string>
#include <utility>
#include <vector>

namespace sup
{
namespace oac_tree
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
   * @brief Asynchronous, i.e. non-blocking, interface to retrieve user input.
   *
   * @note Upon destruction of the returned future, the request is cancelled, but it will not join
   * any threads that were created during creation. This avoids blocking on these objects'
   * destruction.
   *
   * @param request UserInputRequest object, describing what kind of input is expected.
   *
   * @return A handle to a future that will cancel the request upon destruction.
   */
  virtual std::unique_ptr<IUserInputFuture> RequestUserInput(const UserInputRequest& request) = 0;

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
  std::unique_ptr<IUserInputFuture> RequestUserInput(const UserInputRequest& request) override;
  void Message(const std::string& message) override;
  void Log(int severity, const std::string& message) override;
};

sup::dto::AnyValue CreateUserChoiceMetadata();

bool IsUserChoiceMetadata(const sup::dto::AnyValue& metadata);

std::string GetMainTextFromMetadata(const sup::dto::AnyValue& metadata);

/**
 * @brief Get an AnyValue from the user. This function will block until a value is retrieved
 * from the underlying future and is mainly used for testing.
 *
 * @param ui UserInterface object to use for getting user input.
 * @param value Value that indicates the type of value that is requested.
 * @param description Description that will be provided to the user.
 * @return A boolean indicating success of the user input and if true, the user's input as an
 * AnyValue.
 */
std::pair<bool, sup::dto::AnyValue> GetBlockingUserValue(UserInterface& ui,
                                                         const sup::dto::AnyValue& value,
                                                         const std::string& description);

/**
 * @brief Get an integer from the user that indicates a choice. This function will block until a
 * value is retrieved from the underlying future and is mainly used for testing.
 *
 * @param ui UserInterface object to use for getting user input.
 * @param options List of options the user can choose.
 * @param metadata Structure that encodes optional metadata.
 * @return A boolean indicating success of the user input and if true, the user's input as an
 * integer choice.
 */
std::pair<bool, int> GetBlockingUserChoice(UserInterface& ui,
                                           const std::vector<std::string>& options,
                                           const sup::dto::AnyValue& metadata);

/**
 * @brief Create a custom future for retrieving a user input value.
 *
 * @param ui UserInterface object to use for getting user input.
 * @param instr Instruction to check for halted condition.
 * @param value Value that indicates the type of value that is requested.
 * @param description Description that will be provided to the user.
 * @return Unique pointer to a future object or empty if no future could be obtained.
 */
std::unique_ptr<IUserInputFuture> CreateUserValueFuture(
  UserInterface& ui, const Instruction& instr, const sup::dto::AnyValue& value,
  const std::string& description);

/**
 * @brief Create a custom future for retrieving a user choice.
 *
 * @param ui UserInterface object to use for getting user input.
 * @param instr Instruction to check for halted condition.
 * @param options List of options the user can choose.
 * @param metadata Structure that encodes optional metadata.
 * @return A boolean indicating success of the user input and if true, the user's input as an
 * integer choice.
 */
std::unique_ptr<IUserInputFuture> CreateUserChoiceFuture(
  UserInterface& ui, const Instruction& instr, const std::vector<std::string>& options,
  const sup::dto::AnyValue& metadata);

std::pair<bool, int> GetInterruptableUserChoice(UserInterface& ui, const Instruction& instr,
                                                const std::vector<std::string>& options,
                                                const sup::dto::AnyValue& metadata);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_USER_INTERFACE_H_
