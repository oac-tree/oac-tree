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
#include <sup/sequencer/instruction_utils.h>
#include <sup/sequencer/generic_utils.h>

#include <sup/dto/anyvalue_helper.h>

const std::string TIMEOUT_ATTR_NAME = "timeout";
const std::string VARNAME_ATTRIBUTE = "varName";
const std::string EQUALVAR_ATTRIBUTE = "equalsVar";

const double MAX_TIMEOUT_SECONDS = 18.4e9;  // More than 500 years. This should be enough...

namespace sup
{
namespace sequencer
{
const std::string WaitForVariable::Type = "WaitForVariable";

WaitForVariable::WaitForVariable()
  : Instruction(WaitForVariable::Type)
  , m_timeout(0)
{
  AddAttributeDefinition(VARNAME_ATTRIBUTE, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(TIMEOUT_ATTR_NAME, sup::dto::Float64Type).SetMandatory();
  AddAttributeDefinition(EQUALVAR_ATTRIBUTE, sup::dto::StringType);
}

WaitForVariable::~WaitForVariable() = default;

void WaitForVariable::SetupImpl(const Procedure&)
{
  m_timeout = instruction_utils::GetTimeoutFromAttribute(*this, TIMEOUT_ATTR_NAME);
}

ExecutionStatus WaitForVariable::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  int dummy_listener;  // to get a unique address
  std::mutex mx;
  std::condition_variable cv;

  // Register callbacks:
  auto cb_guard = ws.GetCallbackGuard(&dummy_listener);
  RegisterCallback(ws, cv, &dummy_listener, GetAttributeValue<std::string>(VARNAME_ATTRIBUTE));
  if (HasAttribute(EQUALVAR_ATTRIBUTE))
  {
    RegisterCallback(ws, cv, &dummy_listener, GetAttributeValue<std::string>(EQUALVAR_ATTRIBUTE));
  }

  // Wait for condition to be satisfied, halt or timeout:
  bool success = false;
  auto predicate = [&]{
                        success = CheckCondition(ws);
                        return IsHaltRequested() || success;
                      };
  std::unique_lock<std::mutex> lk(mx);
  auto result = cv.wait_for(lk, std::chrono::nanoseconds(m_timeout), predicate);

  return success ? ExecutionStatus::SUCCESS
                 : ExecutionStatus::FAILURE;
}

void WaitForVariable::RegisterCallback(Workspace& ws, std::condition_variable& cv,
                                       void* listener, const std::string& var_name) const
{
  auto callback = [&](const sup::dto::AnyValue&, bool)
                  {
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

bool WaitForVariable::CheckCondition(Workspace& ws) const
{
  sup::dto::AnyValue var_value;
  sup::dto::AnyValue other_value;

  bool var_available = ws.GetValue(GetAttributeValue<std::string>(VARNAME_ATTRIBUTE), var_value) &&
                      !sup::dto::IsEmptyValue(var_value);
  bool other_available;
  if (HasAttribute(EQUALVAR_ATTRIBUTE))
  {
    other_available = ws.GetValue(GetAttributeValue<std::string>(EQUALVAR_ATTRIBUTE), other_value) &&
                      !sup::dto::IsEmptyValue(other_value);
  }
  return SuccessCondition(var_available, var_value, other_available, other_value);
}

}  // namespace sequencer

}  // namespace sup
