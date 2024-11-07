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

#include <sup/sequencer/user_input_request.h>

#include <sup/sequencer/anyvalue_utils.h>
#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>

#include <sup/dto/anyvalue_helper.h>

namespace sup
{
namespace sequencer
{
bool operator==(const UserInputRequest& left, const UserInputRequest& right)
{
  if (left.m_request_type != right.m_request_type)
  {
    return false;
  }
  if (left.m_meta_data != right.m_meta_data)
  {
    return false;
  }
  return left.m_input_type == right.m_input_type;
}

bool operator!=(const UserInputRequest& left, const UserInputRequest& right)
{
  return !(left == right);
}

UserInputRequest CreateUserValueRequest(const sup::dto::AnyValue& value,
                                        const std::string& description)
{
  sup::dto::AnyValue meta_data = description;
  UserInputRequest input_request{ InputRequestType::kUserValue, meta_data, value.GetType() };
  return input_request;
}

UserInputRequest CreateUserChoiceRequest(const std::vector<std::string>& options,
                                         const sup::dto::AnyValue& metadata)
{
  sup::dto::AnyValue options_av{ options.size(), sup::dto::StringType };
  for (std::size_t idx = 0; idx < options.size(); ++idx)
  {
    options_av[idx] = options[idx];
  }
  sup::dto::AnyValue meta = {{
    { Constants::kInputRequestOptionsFieldName, options_av },
    { Constants::kInputRequestMetadataFieldName, metadata }
  }};
  UserInputRequest input_request{ InputRequestType::kUserChoice, meta,
                                  sup::dto::SignedInteger32Type};
  return input_request;
}

bool ParseUserValueRequest(const UserInputRequest& request, sup::dto::AnyValue& value,
                           std::string& description)
{
  if (request.m_request_type != InputRequestType::kUserValue)
  {
    return false;
  }
  if (request.m_meta_data.GetType() != sup::dto::StringType)
  {
    return false;
  }
  if (!sup::dto::TryAssign(value, sup::dto::AnyValue{request.m_input_type}))
  {
    return false;
  }
  description = request.m_meta_data.As<std::string>();
  return true;
}

bool ParseUserChoiceRequest(const UserInputRequest& request, std::vector<std::string>& options,
                            sup::dto::AnyValue& metadata)
{
  if (request.m_request_type != InputRequestType::kUserChoice)
  {
    return false;
  }
  if (!request.m_meta_data.HasField(Constants::kInputRequestOptionsFieldName))
  {
    return false;
  }
  if (!request.m_meta_data.HasField(Constants::kInputRequestMetadataFieldName))
  {
    return false;
  }
  auto& options_av = request.m_meta_data[Constants::kInputRequestOptionsFieldName];
  if (!sup::dto::IsArrayValue(options_av) ||
      options_av.GetType().ElementType() != sup::dto::StringType)
  {
    return false;
  }
  if (!sup::dto::TryAssign(metadata,
                           request.m_meta_data[Constants::kInputRequestMetadataFieldName]))
  {
    return false;
  }
  std::vector<std::string> options_loc;
  for (std::size_t idx = 0; idx < options_av.NumberOfElements(); ++idx)
  {
    options_loc.push_back(options_av[idx].As<std::string>());
  }
  options = std::move(options_loc);
  return true;
}

}  // namespace sequencer

}  // namespace sup
