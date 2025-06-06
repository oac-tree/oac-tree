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

#include <sup/oac-tree/procedure_context.h>

#include <sup/oac-tree/instructions/instruction_helper.h>
#include <sup/oac-tree/procedure/procedure_store.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/procedure.h>

namespace sup
{
namespace oac_tree
{
ProcedureContext::ProcedureContext(const std::string& filename,
                                   const ProcedureStore& procedure_store)
  : m_procedure_filename{filename}
  , m_procedure_store{std::addressof(procedure_store)}
{}

ProcedureContext::~ProcedureContext() = default;

std::string ProcedureContext::GetFilename() const
{
  return m_procedure_filename;
}

const Procedure& ProcedureContext::GetProcedure(const std::string& filename) const
{
  return m_procedure_store->LoadProcedure(filename);
}

Workspace& ProcedureContext::GetWorkspace(const std::string& filename) const
{
  auto& proc = m_procedure_store->LoadProcedure(filename);
  return proc.GetWorkspace();
}

std::unique_ptr<Instruction> ProcedureContext::CloneInstructionPath(
  const std::string& filename, const std::string& path) const
{
  auto& proc = m_procedure_store->LoadProcedure(filename);
  auto top_instructions = proc.GetTopInstructions();
  auto instr = path.empty() ? proc.RootInstruction()
                            : InstructionHelper::FindInstruction(top_instructions, path);
  return InstructionHelper::CloneInstruction(instr);
}

std::unique_ptr<Instruction> CloneInstructionTree(const Instruction& instruction)
{
  return InstructionHelper::CloneInstruction(std::addressof(instruction));
}

}  // namespace oac_tree

}  // namespace sup
