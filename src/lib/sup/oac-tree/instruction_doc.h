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

#ifndef SUP_OAC_TREE_INSTRUCTION_DOC_H_
#define SUP_OAC_TREE_INSTRUCTION_DOC_H_

#include <map>
#include <string>

namespace sup
{
namespace oac_tree
{
/**
 * @brief The InstructionDoc class represents basic information about an instruction. Instances of
 * this class are expected to be registered together with instruction factory functions, to allow
 * user interfaces to display basic documentation about specific instructions.
 */
class InstructionDoc
{
public:
  InstructionDoc();
  explicit InstructionDoc(std::map<std::string, std::string> doc_map);
  ~InstructionDoc();

  // Copy/move construction/assignment
  InstructionDoc(const InstructionDoc& other) = default;
  InstructionDoc(InstructionDoc&& other) noexcept = default;
  InstructionDoc& operator=(const InstructionDoc& other) = default;
  InstructionDoc& operator=(InstructionDoc&& other) noexcept = default;

  std::string GetField(const std::string& field_name) const;

private:
  std::map<std::string, std::string> m_doc_map;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_INSTRUCTION_DOC_H_
