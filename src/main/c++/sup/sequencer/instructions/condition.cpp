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

#include "condition.h"

#include <sup/sequencer/log.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue.h>

namespace sup
{
namespace sequencer
{
const std::string Condition::Type = "Condition";

Condition::Condition() : Instruction(Condition::Type) {}

Condition::~Condition() = default;

ExecutionStatus Condition::ExecuteSingleImpl(UserInterface *, Workspace *ws)
{
  sup::dto::AnyValue var;
  std::string varName = GetAttribute("var_name");
  sup::dto::boolean result = false;
  if (ws->GetValue(varName, var) && var.As(result))
  {
    return result ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup
