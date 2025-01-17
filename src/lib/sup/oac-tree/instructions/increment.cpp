/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) oac-tree component
 *
 * Description   : Instruction node implementation
 *
 * Author        : Ricardo Torres (EXT)
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

#include "increment.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_operations.h>

namespace sup
{
namespace oac_tree
{

const std::string Increment::Type = "Increment";

Increment::Increment()
  : Instruction(Increment::Type)
{
  AddAttributeDefinition(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
}

Increment::~Increment() = default;

ExecutionStatus Increment::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue value;
  if (!GetAttributeValue(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, value))
  {
    return ExecutionStatus::FAILURE;
  }
  if (!sup::dto::Increment(value))
  {
    const std::string warning = InstructionWarningProlog(*this) +
      "could not increment variable reffered to in attribute [" +
      Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME + "]";
    LogWarning(ui, warning);
    return ExecutionStatus::FAILURE;
  }
  if (!SetValueFromAttributeName(*this, ws, ui, Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME,
                                 value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace oac_tree

}  // namespace sup
