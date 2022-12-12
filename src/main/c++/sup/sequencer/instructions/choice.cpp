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

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue_helper.h>

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
  , m_instruction_list{}
{}

Choice::~Choice()
{}

void Choice::SetupImpl(const Procedure &proc)
{
  SetupChildren(proc);
  if (!HasAttribute(SELECTOR_VARIABLE_ATTR_NAME))
  {
    std::string error_message = InstructionSetupExceptionProlog(GetName(), Type) +
      "missing mandatory attribute [" + SELECTOR_VARIABLE_ATTR_NAME + "]";
    throw InstructionSetupException(error_message);
  }
}

ExecutionStatus Choice::ExecuteSingleImpl(UserInterface *ui, Workspace *ws)
{
  if (m_instruction_list.empty() && !CreateInstructionList(ui, ws))
  {
    return ExecutionStatus::FAILURE;
  }
  // If the index list was correctly parsed to be empty, return success
  if (m_instruction_list.empty())
  {
    return ExecutionStatus::SUCCESS;
  }
  for (auto instruction : m_instruction_list)
  {
    auto child_status = instruction->GetStatus();
    if (child_status == ExecutionStatus::SUCCESS)
    {
      continue;
    }
    if (NeedsExecute(child_status))
    {
      instruction->ExecuteSingle(ui, ws);
      break;
    }
  }
  return CalculateCompoundStatus();
}

void Choice::ResetHook()
{
  m_instruction_list.clear();
  ResetChildren();
}

bool Choice::CreateInstructionList(UserInterface *ui, Workspace *ws)
{
  sup::dto::AnyValue selector;
  if (!ws->GetValue(GetAttribute(SELECTOR_VARIABLE_ATTR_NAME), selector))
  {
    std::string error_message = InstructionErrorLogProlog(GetName(), Type) +
      "could not read selector variable with name [" + GetAttribute(SELECTOR_VARIABLE_ATTR_NAME) +
      "] from workspace";
    ui->LogError(error_message);
    return false;
  }
  std::vector<std::size_t> indices;
  if (!GetIndexListFromVariable(indices, selector))
  {
    auto selector_json = sup::dto::ValuesToJSONString(selector);
    std::string error_message = InstructionErrorLogProlog(GetName(), Type) +
      "could not parse selector variable as index or array of indices: [" + selector_json + "]";
    ui->LogError(error_message);
    return false;
  }
  std::vector<Instruction*> instr_list;
  auto child_instructions = ChildInstructions();
  for (auto idx : indices)
  {
    if (idx >= child_instructions.size())
    {
      std::string error_message = InstructionErrorLogProlog(GetName(), Type) +
        "index [" + std::to_string(idx) + "] out of bounds for number of child instructions [" +
        std::to_string(child_instructions.size()) + "]";
      ui->LogError(error_message);
      return false;
    }
    instr_list.push_back(child_instructions[idx]);
  }
  std::swap(m_instruction_list, instr_list);
  return true;
}

ExecutionStatus Choice::CalculateCompoundStatus() const
{
  for (auto instruction : m_instruction_list)
  {
    auto child_status = instruction->GetStatus();

    if (child_status == ExecutionStatus::SUCCESS)
    {
      continue;
    }

    if (child_status == ExecutionStatus::NOT_STARTED
        || child_status == ExecutionStatus::NOT_FINISHED)
    {
      return ExecutionStatus::NOT_FINISHED;
    }
    else
    {
      // Forward RUNNING and FAILURE status of child instruction.
      return child_status;
    }
  }
  return ExecutionStatus::SUCCESS;
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
