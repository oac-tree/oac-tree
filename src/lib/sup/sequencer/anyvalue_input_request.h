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

#ifndef SUP_SEQUENCER_ANYVALUE_INPUT_REQUEST_H_
#define SUP_SEQUENCER_ANYVALUE_INPUT_REQUEST_H_

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
 * @brief AnyValueInputRequest collects all the required information for the supported user
 * input requests.
 */
struct AnyValueInputRequest
{
public:
  InputRequestType m_request_type;
  sup::dto::AnyValue m_meta_data;
  sup::dto::AnyType m_input_type;
};

bool operator==(const AnyValueInputRequest& left, const AnyValueInputRequest& right);
bool operator!=(const AnyValueInputRequest& left, const AnyValueInputRequest& right);

AnyValueInputRequest CreateUserValueRequest(const sup::dto::AnyValue& value,
                                            const std::string& description);

AnyValueInputRequest CreateUserChoiceRequest(const std::vector<std::string>& options,
                                             const sup::dto::AnyValue& metadata);

bool ParseUserValueRequest(const AnyValueInputRequest& request, sup::dto::AnyValue& value,
                           std::string& description);

bool ParseUserChoiceRequest(const AnyValueInputRequest& request, std::vector<std::string>& options,
                            sup::dto::AnyValue& metadata);

sup::dto::AnyValue CreateUserValueReply(bool result, const sup::dto::AnyValue& value);

sup::dto::AnyValue CreateUserChoiceReply(bool result, int choice);

std::pair<bool, sup::dto::AnyValue> ParseUserValueReply(const sup::dto::AnyValue& reply);

std::pair<bool, int> ParseUserChoiceReply(const sup::dto::AnyValue& reply);

/**
 * @brief Pack an AnyValueInputRequest with the given request id into a base64 encoded AnyValue.
 *
 * @param req_id Request id.
 * @param input_request AnyValueInputRequest object to encode.
 *
 * @return Encoded AnyValue.
 */
sup::dto::AnyValue EncodeInputRequest(sup::dto::uint64 req_id,
                                      const AnyValueInputRequest& input_request);

/**
 * @brief Decode the packed and encoded AnyValue into an AnyValueInputRequest.
 *
 * @param encoded Encoded AnyValue.
 *
 * @return Tuple of connected state - request id - AnyValueInputRequest.
 */
std::tuple<bool, sup::dto::uint64, AnyValueInputRequest> DecodeInputRequest(const dto::AnyValue& encoded);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_ANYVALUE_INPUT_REQUEST_H_
