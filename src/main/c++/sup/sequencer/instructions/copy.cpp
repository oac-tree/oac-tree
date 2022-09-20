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

#include <sup/sequencer/workspace.h>

namespace sup
{
namespace sequencer
{
const std::string Copy::Type = "Copy";

ExecutionStatus Copy::ExecuteSingleImpl(UserInterface*, Workspace* ws)
{
  if (!HasAttribute("input") || !HasAttribute("output"))
  {
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue value;
  if (!ws->GetValue(GetAttribute("input"), value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ws->SetValue(GetAttribute("output"), value) ? ExecutionStatus::SUCCESS
                                                     : ExecutionStatus::FAILURE;
}

Copy::Copy() : Instruction(Copy::Type) {}
Copy::~Copy() = default;

}  // namespace sequencer

}  // namespace sup
