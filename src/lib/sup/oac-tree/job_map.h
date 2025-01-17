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

#ifndef SUP_OAC_TREE_JOB_MAP_H_
#define SUP_OAC_TREE_JOB_MAP_H_

#include <sup/oac-tree/instruction_map.h>
#include <sup/oac-tree/variable_map.h>

#include <sup/dto/basic_scalar_types.h>

#include <memory>
#include <string>
#include <vector>

namespace sup
{
namespace oac_tree
{
class Procedure;
class Instruction;

/**
 * @brief JobMap is responsible for the mapping of variable names and instruction pointers to
 * their indices.
 */
class JobMap
{
public:
  explicit JobMap(const Procedure& proc);
  ~JobMap();

  void InitializeInstructionTree(const Instruction* root);

  sup::dto::uint32 GetInstructionIndex(const Instruction* instr) const;

  const InstructionMap& GetInstructionMap() const;

  sup::dto::uint32 GetNumberOfInstructions() const;

  std::vector<const Instruction*> GetOrderedInstructions() const;

  sup::dto::uint32 GetVariableIndex(const std::string& var_name) const;

  const VariableMap& GetVariableMap() const;

  sup::dto::uint32 GetNumberOfVariables() const;

private:
  VariableMap m_variable_map;
  std::unique_ptr<InstructionMap> m_instruction_map;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_JOB_MAP_H_
