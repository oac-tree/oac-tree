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

#ifndef SUP_SEQUENCER_USER_INPUT_REQUEST_H_
#define SUP_SEQUENCER_USER_INPUT_REQUEST_H_

#include <sup/dto/anyvalue.h>

#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace sup
{
namespace sequencer
{

/**
 * @brief Enumeration for the different types of user input.
 *
 */
enum class InputRequestType : sup::dto::uint32
{
  kUserValue = 0,
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

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_USER_INPUT_REQUEST_H_
