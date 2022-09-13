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

// Global header files
#include <common/AnyValueHelper.h>

#include <sup/sequencer/log.h>
#include <chrono>
#include <cmath>

// Local header files

#include <sup/sequencer/Variable.h>

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
  , _setup_successful{false}
{}

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

void Variable::Setup()
{
  _setup_successful = SetupImpl();
}

bool Variable::GetValue(::ccs::types::AnyValue &value, const std::string &fieldname) const
{
  std::lock_guard<std::mutex> lock(_access_mutex);
  if (!_setup_successful)
  {
    log::Warning("Variable::GetValue() - Variable was not successfully set up..");
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
    log::Error("Variable::GetValue() - Failed with field name '%s'", fieldname.c_str());
  }
  return status;
}

bool Variable::SetValue(const ::ccs::types::AnyValue &value, const std::string &fieldname)
{
  std::lock_guard<std::mutex> lk(_access_mutex);
  if (!_setup_successful)
  {
    log::Warning("Variable::SetValue() - Variable was not successfully set up..");
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
    log::Error("Variable::SetValue() - Failed with field name '%s'", fieldname.c_str());
    return false;
  }
  return true;
}

// bool Variable::WaitFor(double seconds) const
// {
//   if (seconds <= 0.0)
//   {
//     return false;
//   }
//   auto duration = std::chrono::nanoseconds(std::lround(seconds * 1e9));
//   std::unique_lock<std::mutex> lk(notify_mutex);
//   auto current_counter = _update_counter;
//   return _update_cond.wait_for(lk, duration,
//       [this, current_counter](){ return _update_counter != current_counter; });
// }

void Variable::Notify(const ccs::types::AnyValue& value)
{
  std::lock_guard<std::mutex> lk(notify_mutex);
  if (notify_cb)
  {
    notify_cb(value);
  }
}

void Variable::SetNotifyCallback(std::function<void(const ccs::types::AnyValue&)> func)
{
  std::lock_guard<std::mutex> lk(notify_mutex);
  notify_cb = std::move(func);
}

void Variable::Reset()
{
  ResetImpl();
  _setup_successful = false;
}

bool Variable::HasAttribute(const std::string &name) const
{
  return _attributes.HasAttribute(name);
}

std::string Variable::GetAttribute(const std::string &name) const
{
  return _attributes.GetAttribute(name);
}

AttributeMap Variable::GetAttributes() const
{
  return _attributes;
}

bool Variable::AddAttribute(const std::string &name, const std::string &value)
{
  std::lock_guard<std::mutex> lock(_access_mutex);
  bool status = _attributes.AddAttribute(name, value);
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
  return status;
}

bool Variable::SetupImpl()
{
  return true;
}

void Variable::ResetImpl() {}

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C
