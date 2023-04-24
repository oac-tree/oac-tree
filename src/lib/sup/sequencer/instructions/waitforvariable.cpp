/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Ricardo Torres (EXT)
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

#include "waitforvariable.h"

#include "sup/sequencer/execution_status.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/generic_utils.h>

#include <chrono>
#include <cmath>
#include <thread>

const std::string TIMEOUT_ATTR_NAME = "timeout";
static const std::string VARNAME_ATTRIBUTE = "varName";
static const std::string EQUALVAR_ATTRIBUTE = "equalsVar";

const double MAX_TIMEOUT_SECONDS = 18.4e9;  // More than 500 years. This should be enough...

namespace sup
{
namespace sequencer
{
const std::string WaitForVariable::Type = "WaitForVariable";

WaitForVariable::WaitForVariable() : Instruction(WaitForVariable::Type), m_timeout(0) {}

WaitForVariable::~WaitForVariable() = default;

void WaitForVariable::SetupImpl(const Procedure&)
{
  CheckMandatoryNonEmptyAttribute(*this, VARNAME_ATTRIBUTE);
  CheckMandatoryNonEmptyAttribute(*this, TIMEOUT_ATTR_NAME);
  double t = InstructionAttributeToDouble(*this, TIMEOUT_ATTR_NAME);
  if (t < 0.0 || t > MAX_TIMEOUT_SECONDS)
  {
    std::string error_message = InstructionSetupExceptionProlog(*this)
                                + "could not convert timeout in seconds [" + std::to_string(t)
                                + "] to nanoseconds (64bit unsigned)";
    throw InstructionSetupException(error_message);
  }
  m_timeout = static_cast<unsigned long>(std::lround(t * 1e9));
}

ExecutionStatus WaitForVariable::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  auto finish = utils::GetNanosecsSinceEpoch() + m_timeout;
  auto var_name = GetAttribute(VARNAME_ATTRIBUTE);
  sup::dto::AnyValue value;
  bool readvar = false;
  bool equalsvarexists = HasAttribute(EQUALVAR_ATTRIBUTE);
  sup::dto::AnyValue value2;
  if (equalsvarexists)
  {
    if (!GetValueFromAttributeName(*this, ws, ui, EQUALVAR_ATTRIBUTE, value2))
    {
      return ExecutionStatus::FAILURE;
    }
  }
  while (!_halt_requested.load() && !readvar && finish > utils::GetNanosecsSinceEpoch())
  {
    readvar = GetValueFromAttributeName(*this, ws, ui, VARNAME_ATTRIBUTE, value);
    if (readvar && equalsvarexists)
    {
      if (value == value2)
      {
        return ExecutionStatus::SUCCESS;
      }
      readvar = false;
    }
  }
  if (_halt_requested.load() || !readvar)
  {
    return ExecutionStatus::FAILURE;
  }
  if (equalsvarexists && !(value == value2))
  {
    return ExecutionStatus::FAILURE;
  }

  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
