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

#include <sup/oac-tree/job_map.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/procedure.h>

namespace sup
{
namespace oac_tree
{

JobMap::JobMap(const Procedure& proc)
  : m_variable_map{proc.GetWorkspace()}
  , m_instruction_map{}
{}

JobMap::~JobMap() = default;

void JobMap::InitializeInstructionTree(const Instruction* root)
{
  m_instruction_map.reset(new InstructionMap{root});
}

sup::dto::uint32 JobMap::GetInstructionIndex(const Instruction* instr) const
{
  if (!m_instruction_map)
  {
    const std::string error = "JobMap::GetInstructionIndex(): object was not "
      "correctly initialized with a root instruction";
    throw InvalidOperationException(error);
  }
  return m_instruction_map->FindInstructionIndex(instr);
}

const InstructionMap& JobMap::GetInstructionMap() const
{
  if (!m_instruction_map)
  {
    const std::string error = "JobMap::GetInstructionMap(): object was not "
      "correctly initialized with a root instruction";
    throw InvalidOperationException(error);
  }
  return *m_instruction_map;
}

sup::dto::uint32 JobMap::GetNumberOfInstructions() const
{
  if (!m_instruction_map)
  {
    return 0;
  }
  return m_instruction_map->GetNumberOfInstructions();
}

std::vector<const Instruction*> JobMap::GetOrderedInstructions() const
{
  if (!m_instruction_map)
  {
    const std::string error = "JobMap::GetOrderedInstructions(): object was not "
      "correctly initialized with a root instruction";
    throw InvalidOperationException(error);
  }
  return GetReverseMap(m_instruction_map->GetInstructionIndexMap());
}

sup::dto::uint32 JobMap::GetVariableIndex(const std::string& var_name) const
{
  return m_variable_map.FindVariableIndex(var_name);
}

const VariableMap& JobMap::GetVariableMap() const
{
  return m_variable_map;
}

sup::dto::uint32 JobMap::GetNumberOfVariables() const
{
  return m_variable_map.GetNumberOfVariables();
}

}  // namespace oac_tree

}  // namespace sup
