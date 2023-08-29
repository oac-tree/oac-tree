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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "var_exists.h"

#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

const std::string VARNAME_ATTRIBUTE = "varName";

namespace sup
{
namespace sequencer
{
const std::string VarExistsInstruction::Type = "VarExists";

VarExistsInstruction::VarExistsInstruction()
  : Instruction(VarExistsInstruction::Type)
{
  AddAttributeDefinition(VARNAME_ATTRIBUTE, sup::dto::StringType).SetMandatory();
}

VarExistsInstruction::~VarExistsInstruction() = default;

ExecutionStatus VarExistsInstruction::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  auto input_field_name = GetAttributeValue<std::string>(VARNAME_ATTRIBUTE);
  if (input_field_name.empty())
  {
    std::string error_message = InstructionErrorProlog(*this) +
      "trying to query existence of variable with empty name";
    ui.LogError(error_message);
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue value;
  if (!ws.GetValue(input_field_name, value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
