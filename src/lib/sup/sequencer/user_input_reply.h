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
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_USER_INPUT_REPLY_H_
#define SUP_SEQUENCER_USER_INPUT_REPLY_H_

#include <sup/sequencer/user_input_request.h>

#include <sup/dto/anyvalue.h>

#include <utility>

namespace sup
{
namespace sequencer
{

/**
 * @brief UserInputReply contains all the required information for the supported user
 * input replies.
 */
struct UserInputReply
{
public:
  InputRequestType m_request_type;
  bool m_result;
  sup::dto::AnyValue m_payload;
};

const UserInputReply kInvalidUserInputReply{ InputRequestType::kInvalid, false, {} };

bool operator==(const UserInputReply& left, const UserInputReply& right);
bool operator!=(const UserInputReply& left, const UserInputReply& right);

UserInputReply CreateUserValueReply(bool result, const sup::dto::AnyValue& value);

UserInputReply CreateUserChoiceReply(bool result, int choice);

std::pair<bool, sup::dto::AnyValue> ParseUserValueReply(const UserInputReply& reply);

std::pair<bool, int> ParseUserChoiceReply(const UserInputReply& reply);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_USER_INPUT_REPLY_H_
