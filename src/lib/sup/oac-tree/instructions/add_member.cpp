/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Carlos Miguel Silva (EXT)
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

#include "add_member.h"

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
const std::string AddMember::Type = "AddMember";

using namespace std;
AddMember::AddMember() : Instruction(AddMember::Type)
{
  AddAttributeDefinition(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
      .SetCategory(AttributeCategory::kVariableName)
      .SetMandatory();
  AddAttributeDefinition(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
      .SetCategory(AttributeCategory::kVariableName)
      .SetMandatory();
  AddAttributeDefinition(Constants::MEMBER_NAME_ATTRIBUTE_NAME).SetMandatory();
}

AddMember::~AddMember() = default;

ExecutionStatus AddMember::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
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

  // Get the name we want to assign to the new var
  auto member_name = GetAttributeString(Constants::MEMBER_NAME_ATTRIBUTE_NAME);
  if (member_name.empty())
  {
    const std::string error =
        InstructionErrorProlog(*this) + " empty member name is not allowed.";
    LogError(ui, error);
    return ExecutionStatus::FAILURE;
  }

  // Check if output_var is a struct
  if (!IsStructValue(output_var))
  {
    const std::string warning =
        InstructionWarningProlog(*this) + " adding members to non-struct variables is not allowed."
        + "Output var [" + GetAttributeString(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
        + "] is not a struct.";
    LogWarning(ui, warning);
    return ExecutionStatus::FAILURE;
  }

  // Check if output_var already contains a member with the same name
  auto has_member_with_name = [&member_name](const std::string& name)
  { return (0 == name.compare(member_name)); };

  auto member_names = output_var.MemberNames();

  if (std::any_of(member_names.begin(), member_names.end(), has_member_with_name))
  {
    const std::string warning = InstructionWarningProlog(*this) + " variable ["
                                + GetAttributeString(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
                                + "] already has a member called ["
                                + GetAttributeString(Constants::MEMBER_NAME_ATTRIBUTE_NAME) + "].";
    LogWarning(ui, warning);
    return ExecutionStatus::FAILURE;
  }

  // Trying to add a member to a locked struct will throw exception
  try
  {
    output_var.AddMember(member_name, input_var);
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
