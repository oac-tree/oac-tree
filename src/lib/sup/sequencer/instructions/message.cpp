/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) Sequencer component
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

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/user_interface.h>

namespace sup
{
namespace sequencer
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

}  // namespace sequencer

}  // namespace sup
