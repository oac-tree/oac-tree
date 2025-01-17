/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) oac-tree component
 *
 * Description   : Instruction node implementation
 *
 * Author        : Walter Van Herck (IO)
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

#include "var_exists.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>

namespace sup
{
namespace oac_tree
{
const std::string VarExistsInstruction::Type = "VarExists";

VarExistsInstruction::VarExistsInstruction()
  : Instruction(VarExistsInstruction::Type)
{
  AddAttributeDefinition(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
}

VarExistsInstruction::~VarExistsInstruction() = default;

ExecutionStatus VarExistsInstruction::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue value;
  if (!GetAttributeValue(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace oac_tree

}  // namespace sup
