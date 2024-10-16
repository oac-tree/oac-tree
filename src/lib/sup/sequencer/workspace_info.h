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

#ifndef SUP_SEQUENCER_WORKSPACE_INFO_H_
#define SUP_SEQUENCER_WORKSPACE_INFO_H_

#include <sup/sequencer/variable_info.h>

#include <sup/dto/basic_scalar_types.h>

#include <string>
#include <utility>
#include <vector>

namespace sup
{
namespace sequencer
{
/**
 * @brief The WorkspaceInfo class represents the static information of a workspace, which consists
 * mainly of map from variable names to VariableInfo objects.
 *
 * @details When objects of this type are created from a real Workspace object, every VariableInfo,
 * representing a single Variable in the Workspace, will receive a unique index from the range
 * [0, N-1], with N the number of Variables. These indices are encoded inside the VariableInfo
 * objects, but are also reflected by their position inside the list that can be retrieved with
 * GetVariableInfos(). This duplication of information allows faster two-way lookup between
 * VariableInfo objects and their indices. The consistency of these indices is ensured at each
 * addition of a new VariableInfo object, together with the uniqueness of their name.
 */
class WorkspaceInfo
{
public:
  WorkspaceInfo();
  ~WorkspaceInfo();

  // Copy/move construction/assignment
  WorkspaceInfo(const WorkspaceInfo& other);
  WorkspaceInfo(WorkspaceInfo&& other);
  WorkspaceInfo& operator=(const WorkspaceInfo& other);
  WorkspaceInfo& operator=(WorkspaceInfo&& other);

  /**
   * @brief Add a new VariableInfo to the list.
   *
   * @param var_name Variable name to use for the VariableInfo object.
   * @param var_info VariableInfo object to add.
   *
   * @throws InvalidOperationException when using a duplicate name or an inconsistent index.
   */
  void AddVariableInfo(const std::string& var_name, const VariableInfo& var_info);

  /**
   * @brief Get the number of registered VariableInfo objects.
   *
   * @return Number of registered VariableInfo objects.
   */
  std::size_t GetNumberOfVariables() const;

  /**
   * @brief Get the list of variable names and their VariableInfo objects.
   *
   * @return List of variable names and their VariableInfo objects.
   */
  const std::vector<std::pair<std::string, VariableInfo>>& GetVariableInfos() const;

private:
  bool ValidateNewVariableInfo(const std::string& var_name, const VariableInfo& var_info) const;
  std::vector<std::pair<std::string, VariableInfo>> m_vars;
};

/**
 * @brief Compare two WorkspaceInfo objects for equality.
 */
bool operator==(const WorkspaceInfo& left, const WorkspaceInfo& right);

/**
 * @brief Compare two WorkspaceInfo objects for inequality.
 */
bool operator!=(const WorkspaceInfo& left, const WorkspaceInfo& right);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_WORKSPACE_INFO_H_
