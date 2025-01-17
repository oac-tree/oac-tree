/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Gennady Pospelov (IO)
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

#include "output.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>

namespace sup
{
namespace oac_tree
{
const std::string Output::Type = "Output";

Output::Output()
  : Instruction(Output::Type)
{
  AddAttributeDefinition(Constants::FROM_VARIABLE_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  AddAttributeDefinition(Constants::DESCRIPTION_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kBoth);
}

Output::~Output() = default;

ExecutionStatus Output::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue value;
  if (!GetAttributeValue(Constants::FROM_VARIABLE_ATTRIBUTE_NAME, ws, ui, value))
  {
    return ExecutionStatus::FAILURE;
  }
  std::string description;
  if (!GetAttributeValueAs(Constants::DESCRIPTION_ATTRIBUTE_NAME, ws, ui, description))
  {
    return ExecutionStatus::FAILURE;
  }
  return ui.PutValue(value, description) ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE;
}

}  // namespace oac_tree

}  // namespace sup
