/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "fallback.h"

namespace sup
{
namespace oac_tree
{
const std::string Fallback::Type = "Fallback";

Fallback::Fallback()
  : CompoundInstruction(Fallback::Type)
{}

Fallback::~Fallback() = default;

ExecutionStatus Fallback::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  for (auto instruction : ChildInstructions())
  {
    auto child_status = instruction->GetStatus();
    if (child_status == ExecutionStatus::FAILURE)
    {
      continue;
    }
    if (NeedsExecute(child_status))
    {
      instruction->ExecuteSingle(ui, ws);
      break;
    }
  }
  return CalculateCompoundStatus();
}

ExecutionStatus Fallback::CalculateCompoundStatus() const
{
  for (auto instruction : ChildInstructions())
  {
    auto child_status = instruction->GetStatus();
    if (child_status == ExecutionStatus::FAILURE)
    {
      continue;
    }
    if (child_status == ExecutionStatus::NOT_STARTED
        || child_status == ExecutionStatus::NOT_FINISHED)
    {
      return ExecutionStatus::NOT_FINISHED;
    }
    return child_status;
  }
  return ExecutionStatus::FAILURE;
}

}  // namespace oac_tree

}  // namespace sup
