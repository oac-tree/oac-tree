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

#ifndef SUP_OAC_TREE_ANYVALUE_UTILS_H_
#define SUP_OAC_TREE_ANYVALUE_UTILS_H_

#include <sup/dto/anyvalue.h>

#include <string>

namespace sup
{
namespace oac_tree
{
namespace utils
{

/**
 * @brief Validate the presence in a given AnyValue of a structure member with the given name
 * and AnyType.
 *
 * @param anyvalue AnyValue to validate.
 * @param mem_name Expected member name.
 * @param mem_type Expected member AnyType.
 * @return true if such a member is present.
 */
bool ValidateMemberType(const sup::dto::AnyValue& anyvalue, const std::string& mem_name,
                        const sup::dto::AnyType& mem_type);

}  // namespace utils

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_ANYVALUE_UTILS_H_
