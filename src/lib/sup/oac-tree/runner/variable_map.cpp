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

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/variable_map.h>
#include <sup/oac-tree/workspace.h>

namespace sup
{
namespace oac_tree
{

VariableMap::VariableMap(const Workspace& workspace)
  : m_variable_map{}
{
  InitializeMap(workspace);
}

VariableMap::~VariableMap() = default;

sup::dto::uint32 VariableMap::FindVariableIndex(const std::string& var_name) const
{
  auto iter = m_variable_map.find(var_name);
  if (iter == m_variable_map.end())
  {
    std::string message = "VariableMap::FindVariableIndex(): unknown variable name [" +
      var_name + "]";
    throw InvalidOperationException(message);
  }
  return iter->second;
}

const VariableMap::VariableIndexMap& VariableMap::GetVariableIndexMap() const
{
  return m_variable_map;
}

sup::dto::uint32 VariableMap::GetNumberOfVariables() const
{
  return m_variable_map.size();
}

void VariableMap::InitializeMap(const Workspace& workspace)
{
  auto var_names = workspace.VariableNames();
  for (sup::dto::uint32 idx = 0; idx < var_names.size(); ++idx)
  {
    m_variable_map[var_names[idx]] = idx;
  }
}

}  // namespace oac_tree

}  // namespace sup
