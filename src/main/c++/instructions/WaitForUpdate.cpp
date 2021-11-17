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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "WaitForUpdate.h"

#include <common/log-api.h>

#include <chrono>
#include <cmath>

#include "Constants.h"
#include "Workspace.h"

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

namespace sup
{
namespace sequencer
{
const std::string WaitForUpdateInstruction::Type = "WaitForUpdate";
static const std::string VARIABLE_NAME_ATTR="variable";

ExecutionStatus WaitForUpdateInstruction::ExecuteSingleImpl(UserInterface*, Workspace* ws)
{
  auto var_name = GetAttribute(VARIABLE_NAME_ATTR);
  auto var = ws->GetVariable(var_name);
  if (var == nullptr)
  {
    log_error("WaitForUpdateInstruction::ExecuteSingleImpl - variable not found");
    return ExecutionStatus::FAILURE;
  }
  auto finish = std::chrono::system_clock::now() +
                std::chrono::nanoseconds(std::lround(timeout * 1e9));
  double accuracy = std::round(DefaultSettings::MAX_BLOCKING_TIME_MS) / 1e3;
  while (!_halt_requested.load() && std::chrono::system_clock::now() < finish)
  {
    if (var->WaitFor(accuracy))
    {
      return ExecutionStatus::SUCCESS;
    }
  }
  return ExecutionStatus::FAILURE;
}

bool WaitForUpdateInstruction::SetupImpl(const Procedure&)
{
  if (!HasAttribute(VARIABLE_NAME_ATTR))
  {
    return false;
  }
  timeout = 0.0;
  if (HasAttribute("timeout"))
  {
    auto timeout_attr = GetAttribute("timeout");
    try
    {
      timeout = std::stod(timeout_attr);
    }
    catch (const std::exception&)
    {
      log_warning("Wait::SetupImpl() - could not parse timeout attribute!");
    }
  }
  return timeout > 0.0;
}

WaitForUpdateInstruction::WaitForUpdateInstruction()
  : Instruction(Type)
  , timeout{0.0}
{}

WaitForUpdateInstruction::~WaitForUpdateInstruction() = default;

}  // namespace sequencer

}  // namespace sup

#undef LOG_ALTERN_SRC
