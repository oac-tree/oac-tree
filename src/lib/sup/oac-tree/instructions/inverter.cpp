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

#include "inverter.h"

namespace sup
{
namespace oac_tree
{
const std::string Inverter::Type = "Inverter";

Inverter::Inverter()
  : DecoratorInstruction(Inverter::Type)
{}

Inverter::~Inverter() = default;

ExecutionStatus Inverter::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  auto child_status = GetChildStatus();
  if (NeedsExecute(child_status))
  {
    ExecuteChild(ui, ws);
  }
  return CalculateStatus();
}

ExecutionStatus Inverter::CalculateStatus() const
{
  auto child_status = GetChildStatus();
  switch (child_status)
  {
  case ExecutionStatus::FAILURE:
    child_status = ExecutionStatus::SUCCESS;
    break;
  case ExecutionStatus::SUCCESS:
    child_status = ExecutionStatus::FAILURE;
    break;
  default:
    break;
  }
  return child_status;
}

}  // namespace oac_tree

}  // namespace sup
