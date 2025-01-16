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

#include <sup/sequencer/workspace_info.h>

#include <sup/sequencer/exceptions.h>

#include <algorithm>

namespace sup
{
namespace sequencer
{

WorkspaceInfo::WorkspaceInfo()
  : m_vars{}
{}

WorkspaceInfo::~WorkspaceInfo() = default;

WorkspaceInfo::WorkspaceInfo(const WorkspaceInfo&) = default;
WorkspaceInfo::WorkspaceInfo(WorkspaceInfo&&) = default;
WorkspaceInfo& WorkspaceInfo::operator=(const WorkspaceInfo&) = default;
WorkspaceInfo& WorkspaceInfo::operator=(WorkspaceInfo&&) = default;

void WorkspaceInfo::AddVariableInfo(const std::string& var_name, const VariableInfo& var_info)
{
  if (!ValidateNewVariableInfo(var_name, var_info))
  {
    const std::string error = "WorkspaceInfo::AddVariableInfo(): trying to add VariableInfo with "
                              "duplicate name or inconsistent index";
    throw InvalidOperationException(error);
  }
  m_vars.emplace_back(var_name, var_info);
}

std::size_t WorkspaceInfo::GetNumberOfVariables() const
{
  return m_vars.size();
}

const std::vector<std::pair<std::string, VariableInfo>>& WorkspaceInfo::GetVariableInfos() const
{
  return m_vars;
}

bool WorkspaceInfo::ValidateNewVariableInfo(const std::string& var_name,
                                            const VariableInfo& var_info) const
{
  auto pred = [var_name](const auto& var) {
    return var.first == var_name;
  };
  auto iter = std::find_if(m_vars.begin(), m_vars.end(), pred);
  if (iter != m_vars.end())
  {
    return false;
  }
  return var_info.GetIndex() == m_vars.size();
}

bool operator==(const WorkspaceInfo& left, const WorkspaceInfo& right)
{
  return left.GetVariableInfos() == right.GetVariableInfos();
}

bool operator!=(const WorkspaceInfo& left, const WorkspaceInfo& right)
{
  return !(left == right);
}

}  // namespace sequencer

}  // namespace sup
