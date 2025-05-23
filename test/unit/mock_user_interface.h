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

#ifndef SUP_OAC_TREE_MOCK_USER_INTERFACE_H_
#define SUP_OAC_TREE_MOCK_USER_INTERFACE_H_

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/user_interface.h>

#include <gmock/gmock.h>

#include <cstring>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Mock UserInterface class for unit testing.
 */
class MockUserInterface : public UserInterface
{
public:
  MOCK_METHOD1(UpdateInstructionStatus, void(const Instruction *));
  MOCK_METHOD3(VariableUpdated,
               void( const std::string& name, const sup::dto::AnyValue& value, bool connected));
  MOCK_METHOD2(PutValue,
               bool(const sup::dto::AnyValue& value, const std::string& description));
  MOCK_METHOD2(GetUserValue,
               bool(sup::dto::AnyValue& value, const std::string& description));
  MOCK_METHOD2(GetUserChoice,
               int(const std::vector<std::string>& options,
                   const sup::dto::AnyValue& metadata));
  MOCK_METHOD1(RequestUserInput, std::unique_ptr<IUserInputFuture>(const UserInputRequest& request));
  MOCK_METHOD1(Message, void(const std::string& message));
  MOCK_METHOD2(Log, void(int severity, const std::string& message));
};

MATCHER_P(HasExecutionStatus, expected, "")
{
  return arg->GetStatus() == expected;
}

MATCHER_P(HasSameValue, value, "")
{
  return arg == value;
}

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_MOCK_USER_INTERFACE_H_
