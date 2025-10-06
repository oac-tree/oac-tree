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

#include <sup/oac-tree/instruction_filters.h>

#include <sup/oac-tree/exceptions.h>

#include <algorithm>

namespace sup
{
namespace oac_tree
{

namespace
{
bool CheckAbsenceOfLoops(const std::vector<sup::dto::uint32>& parent_indices);
void RemoveLineage(std::set<sup::dto::uint32>& input_set,
                   const std::vector<sup::dto::uint32>& lineage);
void AddLineage(std::set<sup::dto::uint32>& result_set,
                const std::vector<sup::dto::uint32>& lineage);
}  // unnamed namespace

AncestorsActiveFilter::AncestorsActiveFilter(std::vector<sup::dto::uint32> parent_indices)
  : m_parent_idx{std::move(parent_indices)}
{
  ValidateParentIndices();
}

AncestorsActiveFilter::~AncestorsActiveFilter() = default;

std::set<sup::dto::uint32> AncestorsActiveFilter::operator()(
  const std::set<sup::dto::uint32>& input_set) const
{
  std::set<sup::dto::uint32> result;
  std::set<sup::dto::uint32> working_set = input_set;

  while (!working_set.empty())
  {
    std::vector<sup::dto::uint32> lineage;
    sup::dto::uint32 current = *working_set.begin();

    while (true)
    {
      lineage.push_back(current);
      auto [status, parent] = GetParent(current);

      if (status == ParentageStatus::kNotFound)
      {
        return {};
      }
      // If the node is the root node or its parent was already decided to be included,
      // we can add the while lineage to the result set.
      if (status == ParentageStatus::kNoParent || result.count(parent) == 1)
      {
        AddLineage(result, lineage);
        break;
      }
      // Parent is in the working set, so continue building the lineage.
      if (working_set.count(parent))
      {
        current = parent;
      }
      // Parent is not active, so the whole lineage can be ignored.
      else
      {
        break;
      }
    }
    RemoveLineage(working_set, lineage);
  }
  return result;
}

void AncestorsActiveFilter::ValidateParentIndices() const
{
  auto size = m_parent_idx.size();
  auto is_in_range = [size](const sup::dto::uint32& idx) {
    return idx < size || idx == kIsRootIndex;
  };
  if (std::find_if_not(m_parent_idx.begin(), m_parent_idx.end(), is_in_range) !=
      m_parent_idx.end())
  {
    throw sup::oac_tree::InvalidOperationException(
      "Parent indices must be in the range [0, size) or equal to kIsRootIndex.");
  }
  auto root_count = std::count(m_parent_idx.begin(), m_parent_idx.end(), kIsRootIndex);
  if (root_count != 1)
  {
    throw sup::oac_tree::InvalidOperationException(
      "There must be exactly one root node in the parent index list.");
  }
  if (!CheckAbsenceOfLoops(m_parent_idx))
  {
    throw sup::oac_tree::InvalidOperationException("Parent index list must not contain loops.");
  }
}

std::pair<AncestorsActiveFilter::ParentageStatus, sup::dto::uint32>
AncestorsActiveFilter::GetParent(const sup::dto::uint32& idx) const
{
  if (idx >= m_parent_idx.size())
  {
    return { ParentageStatus::kNotFound, 0 };
  }
  auto parent_idx = m_parent_idx[idx];
  if (parent_idx == kIsRootIndex)
  {
    return { ParentageStatus::kNoParent, 0 };
  }
  return { ParentageStatus::kValidParent, parent_idx };
}

namespace
{
bool CheckAbsenceOfLoops(const std::vector<sup::dto::uint32>& parent_indices)
{
  std::set<sup::dto::uint32> visited;
  for (sup::dto::uint32 idx = 0; idx < parent_indices.size(); ++idx)
  {
    if (visited.count(idx))
    {
      continue;  // Already visited this node.
    }
    std::set<sup::dto::uint32> current_path;
    sup::dto::uint32 current = idx;

    while (current != kIsRootIndex && !current_path.count(current))
    {
      (void)current_path.insert(current);
      current = parent_indices[current];
    }
    if (current == kIsRootIndex)
    {
      visited.insert(current_path.begin(), current_path.end());
      continue;  // Reached the root, no loop.
    }
    else
    {
      return false;  // Found a loop.
    }
  }
  return true;  // No loops found.
}

void RemoveLineage(std::set<sup::dto::uint32>& input_set,
                   const std::vector<sup::dto::uint32>& lineage)
{
  for (const auto& idx : lineage)
  {
    (void)input_set.erase(idx);
  }
}

void AddLineage(std::set<sup::dto::uint32>& result_set,
                const std::vector<sup::dto::uint32>& lineage)
{
  result_set.insert(lineage.begin(), lineage.end());
}

}  // unnamed namespace

}  // namespace oac_tree

}  // namespace sup
