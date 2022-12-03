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

#include "copy.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/workspace.h>

const std::string INPUT_VARIABLE_ATTR_NAME = "input";
const std::string OUTPUT_VARIABLE_ATTR_NAME = "output";

namespace sup
{
namespace sequencer
{
const std::string Copy::Type = "Copy";

Copy::Copy() : Instruction(Copy::Type) {}
Copy::~Copy() = default;

void Copy::SetupImpl(const Procedure &proc)
{
  if (!HasAttribute(INPUT_VARIABLE_ATTR_NAME) || !HasAttribute(OUTPUT_VARIABLE_ATTR_NAME))
  {
    std::string error_message =
      "sup::sequencer::Copy::SetupImpl(): missing mandatory attributes [" +
       INPUT_VARIABLE_ATTR_NAME + ", " + OUTPUT_VARIABLE_ATTR_NAME + "]";
    throw InstructionSetupException(error_message);
  }
}

ExecutionStatus Copy::ExecuteSingleImpl(UserInterface*, Workspace* ws)
{
  sup::dto::AnyValue value;
  if (!ws->GetValue(GetAttribute(INPUT_VARIABLE_ATTR_NAME), value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ws->SetValue(GetAttribute(OUTPUT_VARIABLE_ATTR_NAME), value) ? ExecutionStatus::SUCCESS
                                                                      : ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup
