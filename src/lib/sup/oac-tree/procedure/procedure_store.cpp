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

#include "procedure_store.h"

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/workspace.h>

namespace sup
{
namespace oac_tree
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

Procedure& ProcedureStore::LoadProcedure(const std::string& filename) const
{
  if (filename == m_parent->GetFilename())
  {
    return *m_parent;
  }
  if (m_procedure_cache.find(filename) == m_procedure_cache.end())
  {
    auto proc = ParseProcedureFile(filename);
    proc->SetParentProcedure(m_parent);
    proc->SetupPreamble();
    m_procedure_cache[filename] = std::move(proc);
  }
  return *m_procedure_cache[filename];
}

void ProcedureStore::ResetProcedureWorkspaces(UserInterface& ui) const
{
  for (auto& [_, procedure] : m_procedure_cache)
  {
    auto& ws = procedure->GetWorkspace();
    ws.Teardown();
    ws.Setup();
  }
}

void ProcedureStore::TearDownProcedures(UserInterface& ui) const
{
  m_procedure_cache.clear();
}

}  // namespace oac_tree

}  // namespace sup
