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

#include <sup/sequencer/variable.h>

#include <sup/sequencer/log.h>

#include <sup/dto/anyvalue_helper.h>

#include <chrono>
#include <cmath>

namespace sup
{
namespace sequencer
{

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

bool Variable::GetValue(sup::dto::AnyValue &value, const std::string &fieldname) const
{
  std::lock_guard<std::mutex> lock(_access_mutex);
  if (!_setup_successful)
  {
    return false;
  }
  if (fieldname.empty())
  {
    return GetValueImpl(value);
  }

  sup::dto::AnyValue var_copy;
  if (GetValueImpl(var_copy))
  {
    try
    {
      value = var_copy[fieldname];
    }
    catch(const sup::dto::InvalidOperationException&)
    {
      return false;
    }
    return true;
  }
  return false;
}

bool Variable::SetValue(const sup::dto::AnyValue &value, const std::string &fieldname)
{
  std::lock_guard<std::mutex> lk(_access_mutex);
  if (!_setup_successful)
  {
    return false;
  }
  if (fieldname.empty())
  {
    return SetValueImpl(value);
  }
  sup::dto::AnyValue var_copy;
  if (GetValueImpl(var_copy))
  {
    try
    {
      var_copy[fieldname] = value;
      return SetValueImpl(var_copy);
    }
    catch(const sup::dto::InvalidOperationException&)
    {
      return false;
    }
  }
  return false;
}

void Variable::Notify(const sup::dto::AnyValue& value)
{
  std::lock_guard<std::mutex> lk(notify_mutex);
  if (notify_cb)
  {
    notify_cb(value);
  }
}

void Variable::SetNotifyCallback(std::function<void(const sup::dto::AnyValue&)> func)
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
