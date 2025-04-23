/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) oac-tree component
 *
 * Description   : Instruction node implementation
 *
 * Author        : Bertrand Bauvir (IO)
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

#include "reset_variable.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>

namespace sup
{
namespace oac_tree
{

const std::string ResetVariable::Type = "ResetVariable";

ResetVariable::ResetVariable()
  : Instruction(ResetVariable::Type)
{
  AddAttributeDefinition(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
}

ResetVariable::~ResetVariable() = default;

ExecutionStatus ResetVariable::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  auto var_name = GetAttributeString(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME);
  if (!ws.HasVariable(var_name))
  {
    std::string error_message = InstructionErrorProlog(*this) +
      "workspace does not contain variable with name [" + var_name + "]";
    LogError(ui, error_message);
    return ExecutionStatus::FAILURE;
  }
  if (!ws.ResetVariable(var_name))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace oac_tree

}  // namespace sup
