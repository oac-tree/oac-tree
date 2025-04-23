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

#ifndef SUP_OAC_TREE_USER_INPUT_REQUEST_H_
#define SUP_OAC_TREE_USER_INPUT_REQUEST_H_

#include <sup/dto/anyvalue.h>

#include <string>
#include <vector>

namespace sup
{
namespace oac_tree
{

/**
 * @brief Enumeration for the different types of user input.
 *
 */
enum class InputRequestType : sup::dto::uint32
{
  kInvalid = 0,
  kUserValue,
  kUserChoice
};

/**
 * @brief UserInputRequest collects all the required information for the supported user
 * input requests.
 */
struct UserInputRequest
{
public:
  InputRequestType m_request_type;
  sup::dto::AnyValue m_meta_data;
  sup::dto::AnyType m_input_type;
};

const UserInputRequest kInvalidUserInputRequest{ InputRequestType::kInvalid, {}, {} };

bool operator==(const UserInputRequest& left, const UserInputRequest& right);
bool operator!=(const UserInputRequest& left, const UserInputRequest& right);

UserInputRequest CreateUserValueRequest(const sup::dto::AnyValue& value,
                                        const std::string& description);

UserInputRequest CreateUserChoiceRequest(const std::vector<std::string>& options,
                                         const sup::dto::AnyValue& metadata);

bool ParseUserValueRequest(const UserInputRequest& request, sup::dto::AnyValue& value,
                           std::string& description);

bool ParseUserChoiceRequest(const UserInputRequest& request, std::vector<std::string>& options,
                            sup::dto::AnyValue& metadata);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_USER_INPUT_REQUEST_H_
