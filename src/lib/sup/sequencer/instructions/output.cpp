/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Gennady Pospelov (IO)
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

#include "output.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

const std::string FROM_ATTRIBUTE_NAME{"from"};
const std::string DESCR_ATTRIBUTE_NAME{"description"};

namespace sup
{
namespace sequencer
{
const std::string Output::Type = "Output";

Output::Output()
  : Instruction(Output::Type)
{}

Output::~Output() = default;

void Output::SetupImpl(const Procedure &proc)
{
  CheckMandatoryNonEmptyAttribute(*this, FROM_ATTRIBUTE_NAME);
}

ExecutionStatus Output::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  auto from_field = GetAttribute(FROM_ATTRIBUTE_NAME);
  auto from_var = SplitFieldName(from_field).first;
  if (!ws.HasVariable(from_var))
  {
    std::string error_message = InstructionErrorProlog(*this) +
      "workspace does not contain variable with name [" + from_var + "]";
    ui.LogError(error_message);
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue value;
  if (!ws.GetValue(from_field, value))
  {
    std::string warning_message = InstructionWarningProlog(*this) +
      "could not read field with name [" + from_field + "] from workspace";
    ui.LogWarning(warning_message);
    return ExecutionStatus::FAILURE;
  }
  return ui.PutValue(value, GetAttribute(DESCR_ATTRIBUTE_NAME)) ? ExecutionStatus::SUCCESS
                                                                 : ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup
