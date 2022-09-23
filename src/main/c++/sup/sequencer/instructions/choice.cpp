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
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "choice.h"

#include <sup/sequencer/log.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/workspace.h>

namespace
{
bool GetIndexListFromVariable(std::vector<std::size_t>& list, const sup::dto::AnyValue& var);
}  // unnamed namespace

const std::string SELECTOR_VARIABLE_ATTR_NAME = "varName";

namespace sup
{
namespace sequencer
{

const std::string Choice::Type = "Choice";

Choice::Choice()
  : CompoundInstruction(Choice::Type)
  , m_var_name{}
{}

Choice::~Choice()
{}

bool Choice::SetupImpl(const Procedure &proc)
{
  if (!SetupChildren(proc))
  {
    return false;
  }
  if (!HasAttribute(SELECTOR_VARIABLE_ATTR_NAME))
  {
    return false;
  }
  m_var_name = GetAttribute(SELECTOR_VARIABLE_ATTR_NAME);
  return true;
}

ExecutionStatus Choice::ExecuteSingleImpl(UserInterface *ui, Workspace *ws)
{
  sup::dto::AnyValue selector;
  if (!ws->GetValue(m_var_name, selector))
  {
    return ExecutionStatus::FAILURE;
  }
  std::vector<std::size_t> indices;
  if (!GetIndexListFromVariable(indices, selector))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecuteArraySelector(indices, ui, ws);
}

ExecutionStatus Choice::ExecuteArraySelector(std::vector<std::size_t> indices, UserInterface *ui,
                                             Workspace *ws)
{
  // TODO: change logic like in Sequence, because this will fail when child returns running
  ExecutionStatus child_status = ExecutionStatus::SUCCESS;
  for (auto idx : indices)
  {
    child_status = ExecuteChild(idx, ui, ws);
    // continue only if success
    if (child_status != ExecutionStatus::SUCCESS)
    {
      break;
    }
  }
  return child_status;
}

ExecutionStatus Choice::ExecuteChild(std::size_t idx, UserInterface *ui, Workspace *ws)
{
  auto child_instructions = ChildInstructions();
  if (idx < child_instructions.size())
  {
    auto child_instr = child_instructions[idx];
    if (NeedsExecute(child_instr->GetStatus()))
    {
      auto childName = child_instr->GetName();
      log::Debug("Choice::ExecuteChild - Executing child[%u]=%s", idx, childName.c_str());

      child_instr->ExecuteSingle(ui, ws);
    }
    return child_instr->GetStatus();
  }
  return ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup

namespace
{
bool GetIndexListFromVariable(std::vector<std::size_t>& list, const sup::dto::AnyValue& var)
{
  std::vector<std::size_t> result;
  std::size_t idx;
  if (sup::dto::IsArrayValue(var))
  {
    for (std::size_t i = 0; i < var.NumberOfElements(); ++i)
    {
      if (!var[i].As(idx))
      {
        return false;
      }
      result.push_back(idx);
    }
  }
  else
  {
    if (!var.As(idx))
    {
      return false;
    }
    result.push_back(idx);
  }
  list = std::move(result);
  return true;
}
}  // unnamed namespace
