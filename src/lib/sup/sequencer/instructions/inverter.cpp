/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "inverter.h"

namespace sup
{
namespace sequencer
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
  auto status = child_status;

  switch (child_status)
  {
  case ExecutionStatus::FAILURE:
    status = ExecutionStatus::SUCCESS;
    break;
  case ExecutionStatus::SUCCESS:
    status = ExecutionStatus::FAILURE;
    break;
  default:
    break;
  }
  return status;
}

}  // namespace sequencer

}  // namespace sup
