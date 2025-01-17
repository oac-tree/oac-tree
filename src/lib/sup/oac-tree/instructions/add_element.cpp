/******************************************************************************
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Rui Oliveira (EXT)
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

#include "add_element.h"

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_exceptions.h>
#include <sup/dto/anyvalue_operations.h>
#include <sup/dto/basic_scalar_types.h>
#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>

namespace sup
{
namespace oac_tree
{
const std::string AddElement::Type = "AddElement";

AddElement::AddElement() : Instruction(AddElement::Type)
{
  AddAttributeDefinition(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
      .SetCategory(AttributeCategory::kVariableName)
      .SetMandatory();
  AddAttributeDefinition(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
      .SetCategory(AttributeCategory::kVariableName)
      .SetMandatory();
}

AddElement::~AddElement() = default;

ExecutionStatus AddElement::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue input_var;
  if (!GetAttributeValue(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, input_var))
  {
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue output_var;
  if (!GetAttributeValue(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, output_var))
  {
    return ExecutionStatus::FAILURE;
  }

  // Check if output_var is an array
  if (!sup::dto::IsArrayValue(output_var))
  {
    std::string warning_message =
        InstructionWarningProlog(*this) + " only allowed to insert into an array.";
    LogWarning(ui, warning_message);
    return ExecutionStatus::FAILURE;
  }

  // Check member type
  if (output_var.GetType().ElementType() != input_var.GetType())
  {
    std::string warning_message = InstructionWarningProlog(*this)
                                + " trying to insert wrong element type: " + input_var.GetTypeName()
                                + ". Expected: " + output_var.GetType().ElementType().GetTypeName();
    LogWarning(ui, warning_message);
    return ExecutionStatus::FAILURE;
  }

  try
  {
    output_var.AddElement(input_var);
  }
  catch (const sup::dto::InvalidOperationException& e)
  {
    const std::string warning = InstructionWarningProlog(*this) + e.what();
    LogWarning(ui, warning);
    return ExecutionStatus::FAILURE;
  }

  // Add to WS
  if (!SetValueFromAttributeName(*this, ws, ui, Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME,
                                 output_var))
  {
    return ExecutionStatus::FAILURE;
  }

  return ExecutionStatus::SUCCESS;
}

}  // namespace oac_tree
}  // namespace sup
