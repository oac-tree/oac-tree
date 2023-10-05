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

#include "increment.h"

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_operations.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

namespace sup
{
namespace sequencer
{

const std::string Increment::Type = "Increment";

const std::string VARNAME_ATTRIBUTE = "varName";

Increment::Increment()
  : Instruction(Increment::Type)
{
  AddAttributeDefinition(VARNAME_ATTRIBUTE)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
}

Increment::~Increment() = default;

ExecutionStatus Increment::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue value;
  if (!GetAttributeValue(VARNAME_ATTRIBUTE, ws, ui, value))
  {
    return ExecutionStatus::FAILURE;
  }
  if (!sup::dto::Increment(value))
  {
    const std::string warning = InstructionErrorProlog(*this) +
      "could not increment variable reffered to in attribute [" + VARNAME_ATTRIBUTE + "]";
    ui.LogWarning(warning);
    return ExecutionStatus::FAILURE;
  }
  if (!SetValueFromAttributeName(*this, ws, ui, VARNAME_ATTRIBUTE, value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
