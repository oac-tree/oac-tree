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
  auto time_end = std::chrono::system_clock::now() + std::chrono::nanoseconds(m_timeout);
  auto var_name = GetAttribute(VARNAME_ATTRIBUTE);
  bool equals_var_exists = HasAttribute(EQUALVAR_ATTRIBUTE);
  std::string equals_var_name;

  sup::dto::AnyValue value1;
  sup::dto::AnyValue value2;

  bool read_value1 = false;
  bool read_value2 = false;

  int dummy_listener;  // to get a unique address
  std::mutex mx;
  std::unique_lock<std::mutex> lk(mx);
  std::condition_variable cv;
  auto cb_guard = ws.GetCallbackGuard(&dummy_listener);

  read_value1 = GetValueFromAttributeName(*this, ws, ui, VARNAME_ATTRIBUTE, value1);

  auto callback = [](std::condition_variable& cv, const sup::dto::AnyValue& val, bool boo,
                     sup::dto::AnyValue& value, bool flag)
  {
    value = val;
    flag = boo;
    cv.notify_one();
  };

  ws.RegisterCallback(var_name,
                      std::bind(callback, std::ref(cv), std::placeholders::_1,
                                std::placeholders::_2, std::ref(value1), std::ref(read_value1)),
                      &dummy_listener);

  if (equals_var_exists)
  {
    equals_var_name = GetAttribute(EQUALVAR_ATTRIBUTE);
    read_value2 = GetValueFromAttributeName(*this, ws, ui, EQUALVAR_ATTRIBUTE, value2);

    ws.RegisterCallback(equals_var_name,
                        std::bind(callback, std::ref(cv), std::placeholders::_1,
                                  std::placeholders::_2, std::ref(value2), std::ref(read_value2)),
                        &dummy_listener);
  }

  auto result = cv.wait_until(
      lk, time_end,
      [this, &ws, &ui, &value1, &value2, &read_value1, &read_value2]
      {
        if (!_halt_requested.load() && read_value1)
        {
          if (read_value2)
          {
            if (value1 == value2)
            {
              return true;
            }
            return false;
          }
          return true;
        }
        return false;
      });

  if (result)
    return ExecutionStatus::SUCCESS;
  return ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup
