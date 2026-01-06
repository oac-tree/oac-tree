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
 * Copyright (c) : 2010-2026 ITER Organization,
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
  MOCK_METHOD(void, UpdateInstructionStatus, (const Instruction *, ExecutionStatus), (override));
  MOCK_METHOD(void, VariableUpdated,
              (const std::string& name, const sup::dto::AnyValue& value, bool connected), (override));
  MOCK_METHOD(void, BreakpointInstructionUpdated, (const Instruction* instruction), (override));
  MOCK_METHOD(bool, PutValue, (const sup::dto::AnyValue& value, const std::string& description), (override));
  MOCK_METHOD(std::unique_ptr<IUserInputFuture>, RequestUserInput, (const UserInputRequest& request), (override));
  MOCK_METHOD(void, Message, (const std::string& message), (override));
  MOCK_METHOD(void, Log, (int severity, const std::string& message), (override));
};

MATCHER_P(HasSameValue, value, "")
{
  return arg == value;
}

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_MOCK_USER_INTERFACE_H_
