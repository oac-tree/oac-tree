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

#include <sup/dto/anytype_registry.h>

#include <chrono>
#include <cmath>

namespace sup
{
namespace sequencer
{

Variable::Variable(const std::string &type)
  : m_type{type}
  , m_setup_successful{false}
{}

Variable::~Variable() = default;

std::string Variable::GetType() const
{
  return m_type;
}

std::string Variable::GetName() const
{
  return GetAttribute(attributes::NAME_ATTRIBUTE);
}

void Variable::SetName(const std::string &name)
{
  AddAttribute(attributes::NAME_ATTRIBUTE, name);
}

void Variable::Setup(const sup::dto::AnyTypeRegistry* registry)
{
  if (registry == nullptr)
  {
    sup::dto::AnyTypeRegistry local_registry{};
    m_setup_successful = SetupImpl(local_registry);
  }
  else
  {
    m_setup_successful = SetupImpl(*registry);
  }
}

bool Variable::GetValue(sup::dto::AnyValue &value, const std::string &fieldname) const
{
  std::lock_guard<std::mutex> lock(m_access_mutex);
  if (!m_setup_successful)
  {
    return false;
  }
  sup::dto::AnyValue var_copy;
  if (!GetValueImpl(var_copy) || sup::dto::IsEmptyValue(var_copy))
  {
    return false;
  }
  try
  {
    if (fieldname.empty())
    {
      value = var_copy;
    }
    else
    {
      value = var_copy[fieldname];
    }
  }
  catch(const std::exception& e)
  {
    return false;
  }
  return true;
}

bool Variable::SetValue(const sup::dto::AnyValue &value, const std::string &fieldname)
{
  std::lock_guard<std::mutex> lk(m_access_mutex);
  if (!m_setup_successful)
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
  std::lock_guard<std::mutex> lk(m_notify_mutex);
  if (notify_cb)
  {
    notify_cb(value);
  }
}

void Variable::SetNotifyCallback(std::function<void(const sup::dto::AnyValue&)> func)
{
  std::lock_guard<std::mutex> lk(m_notify_mutex);
  notify_cb = std::move(func);
}

void Variable::Reset()
{
  ResetImpl();
  m_setup_successful = false;
}

bool Variable::HasAttribute(const std::string &name) const
{
  return m_attributes.HasAttribute(name);
}

std::string Variable::GetAttribute(const std::string &name) const
{
  return m_attributes.GetAttribute(name);
}

AttributeMap Variable::GetAttributes() const
{
  return m_attributes;
}

bool Variable::AddAttribute(const std::string &name, const std::string &value)
{
  std::lock_guard<std::mutex> lock(m_access_mutex);
  bool status = m_attributes.AddAttribute(name, value);
  return status;
}

bool Variable::AddAttributes(const AttributeMap &attributes)
{
  std::lock_guard<std::mutex> lock(m_access_mutex);
  bool status = true;
  for (auto &attr : attributes)
  {
    // Order in AND matters: add all attributes, even if previous adding failed.
    status = m_attributes.AddAttribute(attr.first, attr.second) && status;
  }
  return status;
}

bool Variable::SetupImpl(const sup::dto::AnyTypeRegistry&)
{
  return true;
}

void Variable::ResetImpl() {}

}  // namespace sequencer

}  // namespace sup
