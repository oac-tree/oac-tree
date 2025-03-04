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

#ifndef SUP_OAC_TREE_INSTRUCTION_INFO_H_
#define SUP_OAC_TREE_INSTRUCTION_INFO_H_

#include <sup/oac-tree/attribute_info.h>
#include <sup/oac-tree/instruction.h>

#include <sup/dto/basic_scalar_types.h>

#include <memory>
#include <string>
#include <vector>

namespace sup
{
namespace oac_tree
{
/**
 * @brief The InstructionInfo class represents the static information of an Instruction, including
 * its type, attributes and child instructions. It also contains an index that allows to uniquely
 * identify the Instruction within a procedure.
 */
class InstructionInfo
{
public:
  InstructionInfo(const std::string& instr_type, Instruction::Category category,
                  sup::dto::uint32 idx, std::vector<AttributeInfo> attributes);
  ~InstructionInfo();

  // Copy/move construction/assignment
  InstructionInfo(const InstructionInfo& other);
  InstructionInfo(InstructionInfo&& other);
  InstructionInfo& operator=(const InstructionInfo& other);
  InstructionInfo& operator=(InstructionInfo&& other);

  std::string GetType() const;
  Instruction::Category GetCategory() const;
  sup::dto::uint32 GetIndex() const;
  std::vector<AttributeInfo> GetAttributes() const;

  /**
   * @brief Append a child to the end of its list of children.
   *
   * @param child Child InstructionInfo to add.
   *
   * @return Pointer to added child.
   */
  InstructionInfo* AppendChild(std::unique_ptr<InstructionInfo> child);

  std::vector<InstructionInfo*> Children();
  std::vector<const InstructionInfo*> Children() const;

private:
  std::string m_instr_type;
  Instruction::Category m_category;
  sup::dto::uint32 m_index;
  std::vector<AttributeInfo> m_attributes;
  std::vector<std::unique_ptr<InstructionInfo>> m_children;
};

/**
 * @brief Flatten the InstructionInfo tree into a vector of pointers.
 *
 * @param instr_info_tree InstructionInfo tree.
 *
 * @return Flattened list of InstructionInfo pointers.
 * @throws InvalidOperationException when encountering nullptr nodes.
 */
std::vector<const InstructionInfo*> Flatten(const InstructionInfo& instr_info_tree);

/**
 * @brief Validate the consistency of the given InstructionInfo tree. This function checks for
 * nullptr's and unique indices in the correct range. Note that this validation is performed by
 * a separate function, instead of automatically during the creation of a tree, since index
 * validation can only make sense for a complete tree.
 *
 * @param instr_info_tree InstructionInfo tree to validate.
 *
 * @throws InvalidOperationException when validation fails.
 */
void ValidateInstructionInfoTree(const InstructionInfo& instr_info_tree);

/**
 * @brief Compare two InstructionInfo objects for equality.
 */
bool operator==(const InstructionInfo& left, const InstructionInfo& right);

/**
 * @brief Compare two InstructionInfo objects for inequality.
 */
bool operator!=(const InstructionInfo& left, const InstructionInfo& right);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_INSTRUCTION_INFO_H_
