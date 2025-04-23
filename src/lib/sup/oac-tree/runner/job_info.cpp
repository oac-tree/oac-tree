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

#include <sup/oac-tree/job_info.h>

#include <sup/oac-tree/instruction_info_utils.h>

namespace sup
{
namespace oac_tree
{
JobInfo::JobInfo(const std::string& full_name, const WorkspaceInfo& ws_info,
                 std::unique_ptr<InstructionInfo> root_info)
  : m_full_name{full_name}
  , m_ws{ws_info}
  , m_root{std::move(root_info)}
  , m_ordered_instr{utils::CreateOrderedInstructionInfo(*m_root)}
{}

JobInfo::~JobInfo() = default;

JobInfo::JobInfo(const JobInfo& other)
  : m_full_name{other.m_full_name}
  , m_ws{other.m_ws}
  , m_root{new InstructionInfo{*other.m_root}}
  , m_ordered_instr{utils::CreateOrderedInstructionInfo(*m_root)}
{}

JobInfo::JobInfo(JobInfo&&) = default;

JobInfo& JobInfo::operator=(const JobInfo& other)
{
  JobInfo copy{other};
  return this->operator=(std::move(copy));
}

JobInfo& JobInfo::operator=(JobInfo&&) = default;

std::string JobInfo::GetProcedureName() const
{
  return m_full_name;
}

sup::dto::uint32 JobInfo::GetNumberOfVariables() const
{
  return m_ws.GetNumberOfVariables();
}

sup::dto::uint32 JobInfo::GetNumberOfInstructions() const
{
  return m_ordered_instr.size();
}

const WorkspaceInfo& JobInfo::GetWorkspaceInfo() const
{
  return m_ws;
}

const InstructionInfo* JobInfo::GetRootInstructionInfo() const
{
  return m_root.get();
}

bool operator==(const JobInfo& left, const JobInfo& right)
{
  return ( (left.GetProcedureName() == right.GetProcedureName())
        && (left.GetWorkspaceInfo() == right.GetWorkspaceInfo())
        && (*left.GetRootInstructionInfo() == *right.GetRootInstructionInfo()));
}

bool operator!=(const JobInfo& left, const JobInfo& right)
{
  return !(left == right);
}

}  // namespace oac_tree

}  // namespace sup
