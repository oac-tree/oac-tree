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

#include <sup/sequencer/variable.h>

#include <sup/sequencer/exceptions.h>

#include <sup/dto/anytype_registry.h>
#include <sup/dto/anyvalue_helper.h>

#include <chrono>
#include <cmath>

namespace
{
bool TryStrictAssignment(sup::dto::AnyValue& dest, const std::string& dest_field,
                         const sup::dto::AnyValue& src, const std::string& src_field);
}  // unnamed namespace

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
  sup::dto::AnyTypeRegistry local_registry{};
  auto& reg = registry == nullptr ? local_registry : *registry;
  SetupImpl(reg);
  m_setup_successful = true;
}

bool Variable::GetValue(sup::dto::AnyValue &value, const std::string &fieldname) const
{
  std::lock_guard<std::mutex> lock(m_access_mutex);
  if (!m_setup_successful)
  {
    return false;
  }
  sup::dto::AnyValue var_copy;
  if (!GetValueImpl(var_copy))
  {
    return false;
  }
  return TryStrictAssignment(value, {}, var_copy, fieldname);
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
  if (!GetValueImpl(var_copy))
  {
    return false;
  }
  if (!TryStrictAssignment(var_copy, fieldname, value, {}))
  {
    return false;
  }
  return SetValueImpl(var_copy);
}

bool Variable::IsAvailable() const
{
  std::lock_guard<std::mutex> lk(m_access_mutex);
  if (!m_setup_successful)
  {
    return false;
  }
  return IsAvailableImpl();
}

void Variable::Notify(const sup::dto::AnyValue& value, bool connected)
{
  std::lock_guard<std::mutex> lk(m_notify_mutex);
  if (m_notify_cb)
  {
    m_notify_cb(value, connected);
  }
}

void Variable::SetNotifyCallback(Callback func)
{
  std::lock_guard<std::mutex> lk(m_notify_mutex);
  m_notify_cb = std::move(func);
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

bool Variable::AddAttributes(const AttributeMap& attributes)
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

bool Variable::IsAvailableImpl() const
{
  return true;
}

void Variable::SetupImpl(const sup::dto::AnyTypeRegistry&)
{}

void Variable::ResetImpl()
{}

std::string VariableSetupExceptionProlog(const Variable& variable)
{
  auto var_name = variable.GetName();
  auto var_type = variable.GetType();
  return "Setup of variable [" + var_name + "] of type <" + var_type + "> failed: ";
}

void CheckMandatoryAttribute(const Variable& variable, const std::string& attr_name)
{
  if (!variable.HasAttribute(attr_name))
  {
    std::string error_message = VariableSetupExceptionProlog(variable) +
      "missing mandatory attribute [" + attr_name + "]";
    throw VariableSetupException(error_message);
  }
}

void CheckMandatoryNonEmptyAttribute(const Variable& variable, const std::string& attr_name)
{
  CheckMandatoryAttribute(variable, attr_name);
  if (variable.GetAttribute(attr_name).empty())
  {
    std::string error_message = VariableSetupExceptionProlog(variable) +
      "mandatory attribute [" + attr_name + "] is empty";
    throw VariableSetupException(error_message);
  }
}

}  // namespace sequencer

}  // namespace sup

namespace
{
bool TryStrictAssignment(sup::dto::AnyValue& dest, const std::string& dest_field,
                         const sup::dto::AnyValue& src, const std::string& src_field)
{
  if (!dest_field.empty() && !dest.HasField(dest_field))
  {
    return false;
  }
  if (!src_field.empty() && !src.HasField(src_field))
  {
    return false;
  }
  auto& dest_val = dest_field.empty() ? dest : dest[dest_field];
  const auto& src_val = src_field.empty() ? src : src[src_field];
  if (!sup::dto::IsEmptyValue(dest_val) && dest_val.GetType() != src_val.GetType())
  {
    return false;
  }
  dest_val = src_val;
  return true;
}
}  // unnamed namespace
