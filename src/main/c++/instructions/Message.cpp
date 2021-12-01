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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "Message.h"

#include "Instruction.h"
#include "UserInterface.h"

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

namespace sup
{
namespace sequencer
{

const std::string Message::Type = "Message";

static const std::string TEXT_ATTRIBUTE = "text";

bool Message::SetupImpl(const Procedure& proc)
{
  if (!HasAttribute(TEXT_ATTRIBUTE))
  {
    return false;
  }
  return true;
}

ExecutionStatus Message::ExecuteSingleImpl(UserInterface* ui, Workspace*)
{
  auto message = GetAttribute(TEXT_ATTRIBUTE);
  if (!message.empty())
  {
    ui->Message(message);
  }
  return ExecutionStatus::SUCCESS;
}

Message::Message() : Instruction(Message::Type) {}
Message::~Message() = default;

}  // namespace sequencer

}  // namespace sup

#undef LOG_ALTERN_SRC
