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

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/workspace.h>

#include <cstring>

namespace sup
{
namespace sequencer
{
const std::string Equals::Type = "Equals";

ExecutionStatus Equals::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  (void)ui;
  (void)ws;

  bool status = (Instruction::HasAttribute("lhs") && Instruction::HasAttribute("rhs"));

  sup::dto::AnyValue lhs;
  sup::dto::AnyValue rhs;

  if (status)
  {  // Read lhs from workspace
    status = ws->GetValue(Instruction::GetAttribute("lhs"), lhs);
  }

  if (status)
  {  // Read rhs from workspace
    status = ws->GetValue(Instruction::GetAttribute("rhs"), rhs);
  }

  if (status)
  {
    status = (lhs.GetSize() == rhs.GetSize());
  }

  if (status)
  {
    status = (std::memcmp(lhs.GetInstance(), rhs.GetInstance(), lhs.GetSize()) == 0);
  }

  return (status ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE);
}

Equals::Equals() : Instruction(Equals::Type) {}
Equals::~Equals() = default;

}  // namespace sequencer

}  // namespace sup
