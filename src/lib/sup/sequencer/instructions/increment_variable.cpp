/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) Sequencer component
 *
 * Description   : Instruction node implementation
 *
 * Author        : Ricardo Torres (EXT)
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

#include "increment_variable.h"

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_operations.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>


namespace sup
{
namespace sequencer
{

const std::string IncrementVariable::Type = "IncrementVariable";

static const std::string VARNAME_ATTRIBUTE = "varName";

IncrementVariable::IncrementVariable()
  : Instruction(IncrementVariable::Type)
{}

IncrementVariable::~IncrementVariable() = default;

void IncrementVariable::SetupImpl(const Procedure &proc)
{
  CheckMandatoryNonEmptyAttribute(*this, VARNAME_ATTRIBUTE);
}

ExecutionStatus IncrementVariable::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  auto var_name = GetAttribute(VARNAME_ATTRIBUTE);
  if (!ws.HasVariable(var_name))
  {
    std::string error_message = InstructionErrorProlog(*this) +
      "workspace does not contain variable with name [" + var_name + "]";
    ui.LogError(error_message);
    return ExecutionStatus::FAILURE;
  }

  sup::dto::AnyValue var;
  ws.GetValue(var_name, var);
  if (!sup::dto::Increment(var))
  {
    return ExecutionStatus::FAILURE;
  }
  if (!ws.SetValue(var_name, var))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
