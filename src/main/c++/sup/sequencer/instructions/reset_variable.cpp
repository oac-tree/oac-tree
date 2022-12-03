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

#include "reset_variable.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/workspace.h>

namespace sup
{
namespace sequencer
{

const std::string ResetVariable::Type = "ResetVariable";

static const std::string VARNAME_ATTRIBUTE = "varName";

ResetVariable::ResetVariable()
  : Instruction(ResetVariable::Type)
{}

ResetVariable::~ResetVariable() = default;

void ResetVariable::SetupImpl(const Procedure &proc)
{
  if (!HasAttribute(VARNAME_ATTRIBUTE))
  {
    std::string error_message =
      "sup::sequencer::Copy::SetupImpl(): missing mandatory attribute [" +
       VARNAME_ATTRIBUTE + "]";
    throw InstructionSetupException(error_message);
  }
}

ExecutionStatus ResetVariable::ExecuteSingleImpl(UserInterface*, Workspace* ws)
{
  if (ws->ResetVariable(GetAttribute(VARNAME_ATTRIBUTE)))
  {
    return ExecutionStatus::SUCCESS;
  }
  return ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup
