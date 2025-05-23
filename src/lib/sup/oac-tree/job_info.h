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

#ifndef SUP_OAC_TREE_JOB_INFO_H_
#define SUP_OAC_TREE_JOB_INFO_H_

#include <sup/oac-tree/instruction_info.h>
#include <sup/oac-tree/workspace_info.h>

#include <memory>
#include <string>

namespace sup
{
namespace oac_tree
{
/**
 * @brief The JobInfo object represents all the static information about a procedure, i.e. its
 * Instructions, Workspace and also the prefix that is used for identifying dynamic AnyValues.
 */
class JobInfo
{
public:
  JobInfo(const std::string& full_name, const WorkspaceInfo& ws_info,
          std::unique_ptr<InstructionInfo> root_info);
  ~JobInfo();

  // Copy/move construction/assignment
  JobInfo(const JobInfo& other);
  JobInfo(JobInfo&& other);
  JobInfo& operator=(const JobInfo& other);
  JobInfo& operator=(JobInfo&& other);

  std::string GetProcedureName() const;
  sup::dto::uint32 GetNumberOfVariables() const;
  sup::dto::uint32 GetNumberOfInstructions() const;
  const WorkspaceInfo& GetWorkspaceInfo() const;
  const InstructionInfo* GetRootInstructionInfo() const;

private:
  std::string m_full_name;
  WorkspaceInfo m_ws;
  std::unique_ptr<InstructionInfo> m_root;
  std::vector<const InstructionInfo*> m_ordered_instr;
};

/**
 * @brief Compare two JobInfo objects for equality.
 */
bool operator==(const JobInfo& left, const JobInfo& right);

/**
 * @brief Compare two JobInfo objects for inequality.
 */
bool operator!=(const JobInfo& left, const JobInfo& right);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_JOB_INFO_H_
