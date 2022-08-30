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

// Global header files

// Local header files

#include "Copy.h"

#include "Instruction.h"
#include "InstructionRegistry.h"
#include "Workspace.h"

// Type definition

namespace sup
{
namespace sequencer
{
// Function declaration

// Global variables

const std::string Copy::Type = "Copy";

// Function definition

ExecutionStatus Copy::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  (void)ui;
  (void)ws;

  bool status = (Instruction::HasAttribute("input") && Instruction::HasAttribute("output"));

  ccs::types::AnyValue _value;

  if (status)
  {  // Read from workspace
    status = ws->GetValue(Instruction::GetAttribute("input"), _value);
  }

  if (status)
  {  // Write to workspace
    status = ws->SetValue(Instruction::GetAttribute("output"), _value);
  }

  return (status ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE);
}

Copy::Copy() : Instruction(Copy::Type) {}
Copy::~Copy() = default;

}  // namespace sequencer

}  // namespace sup
