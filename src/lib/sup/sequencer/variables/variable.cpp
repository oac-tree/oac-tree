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
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anytype_registry.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/json_type_parser.h>
#include <sup/dto/json_value_parser.h>

#include <chrono>
#include <cmath>

namespace
{
std::string VariableSetupExceptionMessage(const sup::sequencer::Variable& var,
                                          const std::vector<std::string>& failed_constraints);
}  // unnamed namespace

namespace sup
{
namespace sequencer
{

Variable::Variable(const std::string &type)
  : m_type{type}
  , m_attribute_handler{}
  , m_setup_successful{false}
  , m_access_mutex{}
  , m_notify_mutex{}
  , m_update_cond{}
  , m_notify_cb{}
{
  AddAttributeDefinition(Constants::NAME_ATTRIBUTE_NAME, sup::dto::StringType);
}

Variable::~Variable() = default;

std::string Variable::GetType() const
{
  return m_type;
}

std::string Variable::GetName() const
{
  return GetAttributeString(Constants::NAME_ATTRIBUTE_NAME);
}

void Variable::SetName(const std::string &name)
{
  AddAttribute(Constants::NAME_ATTRIBUTE_NAME, name);
}

void Variable::Setup(const Workspace& ws)
{
  if (!m_attribute_handler.ValidateAttributes())
  {
    auto error_message =
      VariableSetupExceptionMessage(*this, m_attribute_handler.GetFailedConstraints());
    throw VariableSetupException(error_message);
  }
  SetupImpl(ws);
  m_setup_successful = true;
}

bool Variable::GetValue(sup::dto::AnyValue& value, const std::string& fieldname) const
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
  if (!fieldname.empty() && !var_copy.HasField(fieldname))
  {
    return false;
  }
  const auto& src_value = fieldname.empty() ? var_copy : var_copy[fieldname];
  return sup::dto::TryAssignIfEmptyOrConvert(value, src_value);
}

bool Variable::SetValue(const sup::dto::AnyValue& value, const std::string& fieldname)
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
  if (!var_copy.HasField(fieldname))
  {
    return false;
  }
  if (!sup::dto::TryAssign(var_copy[fieldname], value))
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
  m_attribute_handler.ClearFailedConstraints();
  m_setup_successful = false;
}

bool Variable::HasAttribute(const std::string &name) const
{
  return m_attribute_handler.HasStringAttribute(name);
}

std::string Variable::GetAttributeString(const std::string &name) const
{
  return GetStringAttributeValue(m_attribute_handler.GetStringAttributes(), name);
}

const StringAttributeList& Variable::GetStringAttributes() const
{
  return m_attribute_handler.GetStringAttributes();
}

bool Variable::AddAttribute(const std::string &name, const std::string &value)
{
  return m_attribute_handler.AddStringAttribute(name, value);
}

bool Variable::AddAttributes(const StringAttributeList& str_attributes)
{
  bool status = true;
  for (auto &attr : str_attributes)
  {
    // Order in AND matters: add all attributes, even if previous adding failed.
    status = AddAttribute(attr.first, attr.second) && status;
  }
  return status;
}

const std::vector<AttributeDefinition>& Variable::GetAttributeDefinitions() const
{
  return m_attribute_handler.GetAttributeDefinitions();
}

AttributeDefinition& Variable::AddAttributeDefinition(const std::string& attr_name,
                                                      const sup::dto::AnyType& value_type)
{
  return m_attribute_handler.AddAttributeDefinition(attr_name, value_type);
}

void Variable::AddConstraint(Constraint constraint)
{
  return m_attribute_handler.AddConstraint(constraint);
}

bool Variable::IsAvailableImpl() const
{
  return true;
}

void Variable::SetupImpl(const Workspace&)
{}

void Variable::ResetImpl()
{}

std::string VariableSetupExceptionProlog(const Variable& variable)
{
  auto var_name = variable.GetName();
  auto var_type = variable.GetType();
  return "Setup of variable [" + var_name + "] of type <" + var_type + "> failed: ";
}

sup::dto::AnyValue ParseAnyValueAttributePair(const Variable& variable,
                                              const std::string& type_attr_name,
                                              const std::string& value_attr_name,
                                              const sup::dto::AnyTypeRegistry& registry)
{
  if (!variable.HasAttribute(type_attr_name))
  {
    return {};
  }
  auto type_str = variable.GetAttributeString(type_attr_name);
  sup::dto::JSONAnyTypeParser type_parser;
  if (!type_parser.ParseString(type_str, &registry))
  {
    std::string error_message = VariableSetupExceptionProlog(variable) +
      "could not parse attribute [" + type_attr_name + "] with value [" + type_str + "] to an "
      "AnyType";
    throw VariableSetupException(error_message);
  }
  auto parsed_type = type_parser.MoveAnyType();
  if (variable.HasAttribute(value_attr_name))
  {
    auto val_str = variable.GetAttributeString(value_attr_name);
    sup::dto::JSONAnyValueParser value_parser;
    if (!value_parser.TypedParseString(parsed_type, val_str))
    {
    std::string error_message = VariableSetupExceptionProlog(variable) +
      "could not parse attribute [" + value_attr_name + "] with value [" + val_str + "] to an "
      "AnyValue";
    throw VariableSetupException(error_message);
    }
    return value_parser.MoveAnyValue();
  }
  return sup::dto::AnyValue(parsed_type);
}

}  // namespace sequencer

}  // namespace sup

namespace
{
std::string VariableSetupExceptionMessage(const sup::sequencer::Variable& var,
                                          const std::vector<std::string>& failed_constraints)
{
  std::string message = VariableSetupExceptionProlog(var) + "Failed attribute constraint(s):" +
                        sup::sequencer::FormatFailedConstraints(failed_constraints);
  return message;
}

}  // unnamed namespace
