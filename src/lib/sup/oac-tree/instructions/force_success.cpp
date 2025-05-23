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
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "force_success.h"

namespace sup
{
namespace oac_tree
{
const std::string ForceSuccess::Type = "ForceSuccess";

ForceSuccess::ForceSuccess()
  : DecoratorInstruction(ForceSuccess::Type)
{}

ForceSuccess::~ForceSuccess() = default;

ExecutionStatus ForceSuccess::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  ExecuteChild(ui, ws);

  auto status = GetChildStatus();
  if (status == ExecutionStatus::FAILURE)
  {
    status = ExecutionStatus::SUCCESS;
  }
  return status;
}

}  // namespace oac_tree

}  // namespace sup
