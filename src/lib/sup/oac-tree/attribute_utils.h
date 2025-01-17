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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_ATTRIBUTE_UTILS_H_
#define SUP_OAC_TREE_ATTRIBUTE_UTILS_H_

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>

#include <string>
#include <utility>

namespace sup
{
namespace oac_tree
{
namespace utils
{
/**
 * Parse an attribute string to an AnyValue with given type. The first element of the return
 * value encodes success of failure.
*/
std::pair<bool, sup::dto::AnyValue> ParseAttributeString(const sup::dto::AnyType& anytype,
                                                         const std::string& repr);

/**
 * Create an attribute string from an AnyValue. This is the reverse of ParseAttributeString.
*/
std::pair<bool, std::string> CreateAttributeString(const sup::dto::AnyValue& value);
}  // namespace utils

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_ATTRIBUTE_UTILS_H_
