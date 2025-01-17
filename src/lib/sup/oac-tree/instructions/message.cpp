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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "message.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/user_interface.h>

namespace sup
{
namespace oac_tree
{

const std::string Message::Type = "Message";

Message::Message()
  : Instruction(Message::Type)
{
  AddAttributeDefinition(Constants::TEXT_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
}

Message::~Message() = default;

ExecutionStatus Message::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  std::string message;
  if (!GetAttributeValueAs(Constants::TEXT_ATTRIBUTE_NAME, ws, ui, message))
  {
    return ExecutionStatus::FAILURE;
  }
  ui.Message(message);
  return ExecutionStatus::SUCCESS;
}

}  // namespace oac_tree

}  // namespace sup
