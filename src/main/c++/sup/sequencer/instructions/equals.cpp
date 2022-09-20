/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) Sequencer component
 *
 * Description   : Instruction node implementation
 *
 * Author        : Walter Van Herck (IO)
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

#include "equals.h"

#include <sup/sequencer/workspace.h>

namespace sup
{
namespace sequencer
{
const std::string Equals::Type = "Equals";

ExecutionStatus Equals::ExecuteSingleImpl(UserInterface*, Workspace* ws)
{
  if (!HasAttribute("lhs") || !HasAttribute("rhs"))
  {
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue lhs;
  sup::dto::AnyValue rhs;
  if (!ws->GetValue(GetAttribute("lhs"), lhs) || !ws->GetValue(GetAttribute("rhs"), rhs))
  {
    return ExecutionStatus::FAILURE;
  }
  return lhs == rhs ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE;
}

Equals::Equals() : Instruction(Equals::Type) {}
Equals::~Equals() = default;

}  // namespace sequencer

}  // namespace sup
