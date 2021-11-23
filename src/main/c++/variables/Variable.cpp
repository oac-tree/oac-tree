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

// Global header files
#include <common/AnyValueHelper.h>

#include <common/log-api.h>
#include <chrono>
#include <cmath>

// Local header files

#include "Variable.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup
{
namespace sequencer
{
// Global variables

// Function declaration

// Function definition

Variable::Variable(const std::string &type)
  : _type{type}
  , _update_counter{0}
{
  _setup_successful = SetupImpl();
}

Variable::~Variable() = default;

std::string Variable::GetType() const
{
  return _type;
}

std::string Variable::GetName() const
{
  return GetAttribute(attributes::NAME_ATTRIBUTE);
}

void Variable::SetName(const std::string &name)
{
  AddAttribute(attributes::NAME_ATTRIBUTE, name);
}

bool Variable::GetValue(::ccs::types::AnyValue &value, const std::string &fieldname) const
{
  std::lock_guard<std::mutex> lock(_access_mutex);
  if (!_setup_successful)
  {
    log_warning("Variable::GetValue() - Variable was not successfully set up..");
    return false;
  }
  if (fieldname.empty())
  {
    return GetValueImpl(value);
  }

  ::ccs::types::AnyValue var_copy;
  bool status = GetValueImpl(var_copy);

  if (status)
  {
    auto attr_size = ::ccs::HelperTools::GetAttributeSize(var_copy.GetType(), fieldname.c_str());
    status = (!value.GetType() || value.GetSize() == attr_size);
  }
  if (status)
  {
    status = ::ccs::HelperTools::GetAttributeValue(&var_copy, fieldname.c_str(), value);
  }
  if (!status)
  {
    log_error("Variable::GetValue() - Failed with field name '%s'", fieldname.c_str());
  }
  return status;
}

bool Variable::SetValue(const ::ccs::types::AnyValue &value, const std::string &fieldname)
{
  std::unique_lock<std::mutex> lock(_access_mutex);
  if (!_setup_successful)
  {
    log_warning("Variable::SetValue() - Variable was not successfully set up..");
    return false;
  }

  bool status = false;
  if (fieldname.empty())
  {
    status = SetValueImpl(value);
  }
  else
  {
    ::ccs::types::AnyValue var_copy;
    status = GetValueImpl(var_copy);
    if (status)
    {
      status = ::ccs::HelperTools::SetAttributeValue(&var_copy, fieldname.c_str(), value);
    }
    if (status)
    {
      // need to update it in the Variable
      status = SetValueImpl(var_copy);
    }
  }
  if (!status)
  {
    log_error("Variable::SetValue() - Failed with field name '%s'", fieldname.c_str());
    return false;
  }
  lock.unlock();
  Notify();
  return true;
}

bool Variable::WaitFor(double seconds) const
{
  if (seconds <= 0.0)
  {
    return false;
  }
  auto duration = std::chrono::nanoseconds(std::lround(seconds * 1e9));
  std::unique_lock<std::mutex> lk(notify_mutex);
  auto current_counter = _update_counter;
  return _update_cond.wait_for(lk, duration,
      [this, current_counter](){ return _update_counter != current_counter; });
}

void Variable::Notify()
{
  {
    std::lock_guard<std::mutex> lk(notify_mutex);
    ++_update_counter;
  }
  _update_cond.notify_all();
}

void Variable::SetNotifyCallback(std::function<void(const ccs::types::AnyValue&)> func)
{
  std::lock_guard<std::mutex> lk(notify_mutex);
  notify_cb = std::move(func);
}

bool Variable::HasAttribute(const std::string &name) const
{
  return _attributes.HasAttribute(name);
}

std::string Variable::GetAttribute(const std::string &name) const
{
  return _attributes.GetAttribute(name);
}

bool Variable::AddAttribute(const std::string &name, const std::string &value)
{
  std::lock_guard<std::mutex> lock(_access_mutex);
  bool status = _attributes.AddAttribute(name, value);
  _setup_successful = SetupImpl();
  return status;
}

bool Variable::AddAttributes(const AttributeMap &attributes)
{
  std::lock_guard<std::mutex> lock(_access_mutex);
  bool status = true;
  for (auto &attr : attributes)
  {
    // Order in AND matters: add all attributes, even if previous adding failed.
    status = _attributes.AddAttribute(attr.first, attr.second) && status;
  }
  _setup_successful = SetupImpl();
  return status;
}

bool Variable::SetupImpl()
{
  return true;
}

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C

#undef LOG_ALTERN_SRC
