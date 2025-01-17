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

#include <sup/oac-tree/variable_info.h>

namespace sup
{
namespace oac_tree
{

VariableInfo::VariableInfo(const std::string& var_type, sup::dto::uint32 idx,
                           std::vector<AttributeInfo> attributes)
  : m_var_type{var_type}
  , m_index{idx}
  , m_attributes{std::move(attributes)}
{}

VariableInfo::~VariableInfo() = default;

VariableInfo::VariableInfo(const VariableInfo&) = default;
VariableInfo::VariableInfo(VariableInfo&&) = default;
VariableInfo& VariableInfo::operator=(const VariableInfo&) = default;
VariableInfo& VariableInfo::operator=(VariableInfo&&) = default;

std::string VariableInfo::GetType() const
{
  return m_var_type;
}

sup::dto::uint32 VariableInfo::GetIndex() const
{
  return m_index;
}

std::vector<AttributeInfo> VariableInfo::GetAttributes() const
{
  return m_attributes;
}

bool operator==(const VariableInfo& left, const VariableInfo& right)
{
  return (left.GetType() == right.GetType())
      && (left.GetIndex() == right.GetIndex())
      && (left.GetAttributes() == right.GetAttributes());
}

bool operator!=(const VariableInfo& left, const VariableInfo& right)
{
  return !(left == right);
}

}  // namespace oac_tree

}  // namespace sup
