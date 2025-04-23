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

#ifndef SUP_OAC_TREE_ATTRIBUTE_INFO_H_
#define SUP_OAC_TREE_ATTRIBUTE_INFO_H_

#include <sup/oac-tree/constraint.h>

#include <string>
#include <vector>
#include <utility>

namespace sup
{
namespace oac_tree
{
using AttributeInfo = std::pair<std::string, std::string>;

std::vector<AttributeInfo> ToAttributeInfos(const StringAttributeList& attrs);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_ATTRIBUTE_INFO_H_
