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
 * Copyright (c) : 2010-2022 ITER Organization,
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

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/user_interface.h>

namespace sup
{
namespace sequencer
{

const std::string Message::Type = "Message";

static const std::string TEXT_ATTRIBUTE = "text";

ExecutionStatus Message::ExecuteSingleImpl(UserInterface* ui, Workspace*)
{
  std::string message;
  if (HasAttribute(TEXT_ATTRIBUTE))
  {
    message = GetAttribute(TEXT_ATTRIBUTE);
  }
  ui->Message(message);
  return ExecutionStatus::SUCCESS;
}

Message::Message() : Instruction(Message::Type) {}
Message::~Message() = default;

}  // namespace sequencer

}  // namespace sup
