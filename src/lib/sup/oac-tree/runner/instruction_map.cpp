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

#include <sup/oac-tree/instruction_map.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_tree.h>

#include <algorithm>

namespace sup
{
namespace oac_tree
{

InstructionMap::InstructionMap(const Instruction* root)
  : m_instruction_map{}
{
  InitializeMap(root);
}

InstructionMap::~InstructionMap() = default;

sup::dto::uint32 InstructionMap::FindInstructionIndex(const Instruction* instr) const
{
  auto iter = m_instruction_map.find(instr);
  if (iter == m_instruction_map.end())
  {
    std::string message = "InstructionMap::FindInstructionIndex(): unknown instruction";
    throw InvalidOperationException(message);
  }
  return iter->second;
}

const InstructionMap::InstructionIndexMap& InstructionMap::GetInstructionIndexMap() const
{
  return m_instruction_map;
}

sup::dto::uint32 InstructionMap::GetNumberOfInstructions() const
{
  return m_instruction_map.size();
}

void InstructionMap::InitializeMap(const Instruction* root)
{
  if (root == nullptr)
  {
    return;
  }
  auto instr_list = FlattenBFS(CreateFullInstructionTree(root));
  InstructionIndexMap instruction_map;
  for (sup::dto::uint32 idx = 0; idx < instr_list.size(); ++idx)
  {
    const auto* instr = instr_list[idx];
    instruction_map[instr] = idx;
  }
  if (instruction_map.size() != instr_list.size())
  {
    const std::string error =
      "InstructionMap::InitializeMap(): duplicate Instruction pointers found";
    throw InvalidOperationException(error);
  }
  m_instruction_map = std::move(instruction_map);
}

std::vector<const Instruction*> GetReverseMap(
  const InstructionMap::InstructionIndexMap& instr_map)
{
  auto size = instr_map.size();
  std::vector<const Instruction*> result(size, nullptr);
  for (const auto& [instruction, idx] : instr_map)
  {
    if (idx >= result.size())
    {
      const std::string error = "GetReverseMap(): index found that is larger than the map size";
      throw InvalidOperationException(error);
    }
    result[idx] = instruction;
  }
  auto iter = std::find(result.begin(), result.end(), nullptr);
  if (iter != result.end())
  {
      const std::string error = "GetReverseMap(): equal indices found for different instructions";
      throw InvalidOperationException(error);
  }
  return result;
}

}  // namespace oac_tree

}  // namespace sup
