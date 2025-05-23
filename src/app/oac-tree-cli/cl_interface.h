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

#ifndef SUP_OAC_TREE_CL_INTERFACE_H_
#define SUP_OAC_TREE_CL_INTERFACE_H_

#include <sup/oac-tree/async_input_adapter.h>
#include <sup/oac-tree/user_interface.h>

#include <sup/log/basic_logger.h>

#include <mutex>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Class implementing a command line user interface for the oac-tree.
 */
class CLInterface : public UserInterface
{
public:
  CLInterface(const sup::log::BasicLogger& logger);

  ~CLInterface() override;

  void UpdateInstructionStatus(const Instruction* instruction) override;
  void VariableUpdated(const std::string& name, const sup::dto::AnyValue& value,
                           bool connected) override;
  bool PutValue(const sup::dto::AnyValue& value, const std::string& description) override;
  std::unique_ptr<IUserInputFuture> RequestUserInput(const UserInputRequest& request) override;
  void Message(const std::string& message) override;
  void Log(int severity, const std::string& message) override;

private:
  UserInputReply UserInput(const UserInputRequest& request, sup::dto::uint64 id);
  void Interrupt(sup::dto::uint64 id);
  bool GetUserValue(sup::dto::AnyValue& value, const std::string& description);
  int GetUserChoice(const std::vector<std::string>& options,
                    const sup::dto::AnyValue& metadata);
  const sup::log::BasicLogger m_logger;
  AsyncInputAdapter m_input_adapter;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_CL_INTERFACE_H_
