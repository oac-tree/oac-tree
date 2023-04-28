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
#include "sup/sequencer/workspace.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/generic_utils.h>

#include <sup/dto/anyvalue_helper.h>

#include <cmath>

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
  auto var_name = GetAttribute(VARNAME_ATTRIBUTE);

  sup::dto::AnyValue var_value;
  sup::dto::AnyValue other_value;

  int dummy_listener;  // to get a unique address
  std::mutex mx;
  std::condition_variable cv;

  // Register callbacks:
  auto cb_guard = ws.GetCallbackGuard(&dummy_listener);
  bool var_available = ws.GetValue(var_name, var_value);
  RegisterCallback(ws, cv, mx, &dummy_listener, GetAttribute(VARNAME_ATTRIBUTE),
                   var_value, var_available);

  bool other_available = false;
  if (HasAttribute(EQUALVAR_ATTRIBUTE))
  {
    other_available = ws.GetValue(GetAttribute(EQUALVAR_ATTRIBUTE), other_value);
    RegisterCallback(ws, cv, mx, &dummy_listener, GetAttribute(EQUALVAR_ATTRIBUTE),
                     other_value, other_available);
  }
  bool success = false;
  auto predicate = [this, &success, &var_value, &other_value, &var_available, &other_available]
                   {
                     success = SuccessCondition(var_available, var_value, other_available, other_value);
                     return _halt_requested.load() || success;
                   };
  std::unique_lock<std::mutex> lk(mx);
  auto result = cv.wait_for(lk, std::chrono::nanoseconds(m_timeout), predicate);

  return success ? ExecutionStatus::SUCCESS
                 : ExecutionStatus::FAILURE;
}

void WaitForVariable::RegisterCallback(Workspace& ws, std::condition_variable& cv, std::mutex& mtx,
                                       void* listener, const std::string& var_name,
                                       sup::dto::AnyValue& value, bool& available) const
{
  auto callback = [&cv, &mtx, &value, &available](const sup::dto::AnyValue& newval, bool connected)
                  {
                    {
                      std::lock_guard<std::mutex> lk(mtx);
                      available = connected && !sup::dto::IsEmptyValue(newval);
                      (void)sup::dto::TryConvert(value, newval);  // ignore failure
                    }
                    cv.notify_one();
                  };
  ws.RegisterCallback(var_name, callback, listener);
}

bool WaitForVariable::SuccessCondition(
  bool var_available, const sup::dto::AnyValue& var_value,
  bool other_available, const sup::dto::AnyValue& other_value) const
{
  if (!var_available)
  {
    return false;
  }
  if (!HasAttribute(EQUALVAR_ATTRIBUTE))
  {
    return true;
  }
  if (!other_available)
  {
    return false;
  }
  return var_value == other_value;
}

}  // namespace sequencer

}  // namespace sup
