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

#ifndef SUP_OAC_TREE_INSTRUCTION_FILTERS_H_
#define SUP_OAC_TREE_INSTRUCTION_FILTERS_H_

#include <sup/oac-tree/active_instruction_monitor.h>

#include <limits>

namespace sup
{
namespace oac_tree
{

const sup::dto::uint32 kIsRootIndex = std::numeric_limits<sup::dto::uint32>::max();

/**
 * @brief This instruction filter removes active instructions if not all of their ancestors are
 * active too.
 */
class AncestorsActiveFilter
{
public:
  /**
   * @brief Construct a filter object with the given list of parent indices
   *
   * @param parent_indices List of parent inidices for each given index.
   *
   * @note The list of parent indices must contain exactly one element equal to kIsRootIndex,
   * should not contain any loops and all indices must be in the expected range.
   */
  AncestorsActiveFilter(std::vector<sup::dto::uint32> parent_indices);
  ~AncestorsActiveFilter();

  std::set<sup::dto::uint32> operator()(const std::set<sup::dto::uint32>& input_set) const;

private:
  enum class ParentageStatus
  {
    kValidParent = 0,  // Node has a valid parent
    kNoParent,         // Node is a root node and thus has no parent
    kNotFound          // Node was not found
  };

  void ValidateParentIndices() const;
  std::pair<ParentageStatus, sup::dto::uint32> GetParent(const sup::dto::uint32& idx) const;
  std::vector<sup::dto::uint32> m_parent_idx;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_INSTRUCTION_FILTERS_H_
