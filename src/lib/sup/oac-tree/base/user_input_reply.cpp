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

#include <sup/oac-tree/user_input_reply.h>

#include <sup/oac-tree/anyvalue_utils.h>
#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>

#include <sup/dto/anyvalue_helper.h>

namespace sup
{
namespace oac_tree
{
bool operator==(const UserInputReply& left, const UserInputReply& right)
{
  if (left.m_request_type != right.m_request_type)
  {
    return false;
  }
  if (left.m_result != right.m_result)
  {
    return false;
  }
  return left.m_payload == right.m_payload;
}

bool operator!=(const UserInputReply& left, const UserInputReply& right)
{
  return !(left == right);
}

UserInputReply CreateUserValueReply(bool result, const sup::dto::AnyValue& value)
{
  UserInputReply input_reply{ InputRequestType::kUserValue, result, value };
  return input_reply;
}

UserInputReply CreateUserChoiceReply(bool result, int choice)
{
  sup::dto::AnyValue choice_av{ sup::dto::SignedInteger32Type, choice };
  UserInputReply input_reply{ InputRequestType::kUserChoice, result, choice_av };
  return input_reply;
}

std::pair<bool, sup::dto::AnyValue> ParseUserValueReply(const UserInputReply& reply)
{
  std::pair<bool, sup::dto::AnyValue> failure{ false, {} };
  if (reply.m_request_type != InputRequestType::kUserValue || !reply.m_result)
  {
    return failure;
  }
  return { true, reply.m_payload };
}

std::pair<bool, int> ParseUserChoiceReply(const UserInputReply& reply)
{
  std::pair<bool, int> failure{ false, -1 };
  if (reply.m_request_type != InputRequestType::kUserChoice || !reply.m_result)
  {
    return failure;
  }
  if (reply.m_payload.GetType() != sup::dto::SignedInteger32Type)
  {
    return failure;
  }
  auto choice = reply.m_payload.As<sup::dto::int32>();
  return { true, choice };
}

}  // namespace oac_tree

}  // namespace sup
