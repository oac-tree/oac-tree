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

const std::string LEFT_VARIABLE_ATTR_NAME = "lhs";
const std::string RIGHT_VARIABLE_ATTR_NAME = "rhs";

namespace sup
{
namespace sequencer
{
const std::string Equals::Type = "Equals";

ExecutionStatus Equals::ExecuteSingleImpl(UserInterface*, Workspace* ws)
{
  if (!HasAttribute(LEFT_VARIABLE_ATTR_NAME) || !HasAttribute(RIGHT_VARIABLE_ATTR_NAME))
  {
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue lhs;
  sup::dto::AnyValue rhs;
  if (!ws->GetValue(GetAttribute(LEFT_VARIABLE_ATTR_NAME), lhs) ||
      !ws->GetValue(GetAttribute(RIGHT_VARIABLE_ATTR_NAME), rhs))
  {
    return ExecutionStatus::FAILURE;
  }
  return lhs == rhs ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE;
}

Equals::Equals() : Instruction(Equals::Type) {}
Equals::~Equals() = default;

}  // namespace sequencer

}  // namespace sup
