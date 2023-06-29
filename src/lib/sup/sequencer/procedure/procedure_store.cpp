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

#include "procedure_store.h"

#include <sup/sequencer/instructions/instruction_helper.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/workspace.h>

namespace sup
{
namespace sequencer
{
ProcedureStore::ProcedureStore(Procedure* parent)
  : m_parent{parent}
  , m_procedure_cache{}
{
  if (parent == nullptr)
  {
    throw InvalidOperationException("ProcedureStore constuctor called with nullptr");
  }
}

ProcedureStore::~ProcedureStore() = default;

const Procedure& ProcedureStore::GetProcedure(const std::string& filename) const
{
  auto proc = LoadProcedure(filename);
  return *proc;
}

Workspace* ProcedureStore::GetWorkspace(const std::string& filename) const
{
  auto proc = LoadProcedure(filename);
  return proc->GetWorkspace();
}

std::unique_ptr<Instruction> ProcedureStore::CloneInstructionPath(const std::string& filename,
                                                                  const std::string& path) const
{
  auto proc = LoadProcedure(filename);
  auto top_instructions = proc->GetTopInstructions();
  auto instr = path.empty() ? proc->RootInstruction()
                            : InstructionHelper::FindInstruction(top_instructions, path);
  return InstructionHelper::CloneInstruction(instr);
}

void ProcedureStore::ResetProcedures() const
{
  for (auto& entry : m_procedure_cache)
  {
    entry.second->Reset();
  }
}

Procedure* ProcedureStore::LoadProcedure(const std::string& filename) const
{
  if (filename == m_parent->GetFilename())
  {
    return m_parent;
  }
  if (m_procedure_cache.find(filename) == m_procedure_cache.end())
  {
    auto proc = ParseProcedureFile(filename);
    proc->SetParentProcedure(m_parent);
    m_procedure_cache[filename] = std::move(proc);
  }
  return m_procedure_cache[filename].get();
}

}  // namespace sequencer

}  // namespace sup
