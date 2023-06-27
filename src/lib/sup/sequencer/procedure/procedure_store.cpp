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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/sequencer/procedure_store.h>

#include <sup/sequencer/instructions/instruction_helper.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/workspace.h>

namespace sup
{
namespace sequencer
{
ProcedureStore::ProcedureStore()
  : m_procedure_cache{}
{}

ProcedureStore::~ProcedureStore() = default;

const Procedure& ProcedureStore::GetProcedure(const std::string& filename) const
{
  if (m_procedure_cache.find(filename) == m_procedure_cache.end())
  {
    m_procedure_cache[filename] = ParseProcedureFile(filename);
  }
  return *m_procedure_cache[filename];
}

std::unique_ptr<Instruction> ProcedureStore::CloneInstructionPath(const std::string& filename,
                                                                  const std::string& path) const
{
  const auto& proc = GetProcedure(filename);
  auto top_instructions = proc.GetTopInstructions();
  auto instr = InstructionHelper::FindInstruction(top_instructions, path);
  if (instr == nullptr)
  {
    std::string error_message = "ProcedureStore::CloneInstructionPath(): could not clone "
    "instruction tree with filename: [" + filename + "] and path: [" + path + "]";
    throw InvalidOperationException(error_message);
  }
  return InstructionHelper::CloneInstruction(instr);
}

std::unique_ptr<Workspace> ProcedureStore::GetWorkspace(const std::string& filename) const
{
  return {};
}

void ProcedureStore::ClearProcedureCache() const
{
  m_procedure_cache.clear();
}

}  // namespace sequencer

}  // namespace sup
