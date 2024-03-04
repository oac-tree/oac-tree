/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Carlos Miguel Silva (EXT)
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

#include "add_member.h"

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_operations.h>
#include <sup/dto/basic_scalar_types.h>
#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

namespace sup
{
namespace sequencer
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
  AddAttributeDefinition(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME).SetMandatory();
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
  auto var_name = GetAttributeString(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME);
  if (var_name.empty())
  {
    return ExecutionStatus::FAILURE;
  }

  // Check if output_var is a struct
  if (!IsStructValue(output_var))
  {
    return ExecutionStatus::FAILURE;
  }

  // Check if output_var already contains a member with the same name
  auto has_member_with_name = [&var_name](const std::string& name)
  { return (0 == name.compare(var_name)); };

  auto member_names = output_var.MemberNames();

  if (std::any_of(member_names.begin(), member_names.end(), has_member_with_name))
  {
    return ExecutionStatus::FAILURE;
  }

  // Trying to add a member to a locked struct will throw exception
  try
  {
    output_var.AddMember(var_name, input_var);
  }
  catch (const InvalidOperationException& e)
  {
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

}  // namespace sequencer
}  // namespace sup
