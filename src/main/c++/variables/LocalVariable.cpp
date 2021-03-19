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

#include <common/log-api.h>
#include <memory>

// Local header files

#include "LocalVariable.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

const std::string LocalVariable::Type = "Local";
const std::string LocalVariable::JSON_TYPE="type";
const std::string LocalVariable::JSON_VALUE="value";

// Function declaration

// Function definition

LocalVariable::LocalVariable()
  : Variable(LocalVariable::Type)
  , _val{}
{}

LocalVariable::~LocalVariable() {}

bool LocalVariable::GetValueImpl(::ccs::types::AnyValue& value) const
{
  if (!_val.GetType())
  {
    log_warning("sup::sequencer::LocalVariable::GetValue() - not initialized..");
    return false;
  }

  if (!value.GetType() || value.GetSize() == _val.GetSize())
  {
    log_debug("sup::sequencer::LocalVariable::GetValue() - copying value..");
    value = _val;
  }
  else
  {
    log_warning("sup::sequencer::LocalVariable::GetValue() - incompatible types..");
    return false;
  }
  return true;
}

bool LocalVariable::SetValueImpl(const ::ccs::types::AnyValue & value)
{
  if (!_val.GetType() || _val.GetSize() == value.GetSize())
  {
    log_debug("sup::sequencer::LocalVariable::SetValue() - copying value..");
    _val = value;
    return true;
  }
  log_warning("sup::sequencer::LocalVariable::SetValue() - incompatible types..");
  return false;
}

bool LocalVariable::SetupImpl()
{
  bool status = HasAttribute(JSON_TYPE);

  ::ccs::base::SharedReference<::ccs::types::AnyType> local_type;
  std::unique_ptr<::ccs::types::AnyValue> local_value;
  if (status)
  {
    log_debug("LocalVariable::Setup() - parsing json type info..");
    std::string json_type = GetAttribute(JSON_TYPE);
    auto read = ::ccs::HelperTools::Parse(local_type, json_type.c_str());
    status = read > 0;
  }
  else
  {
    log_debug("LocalVariable::Setup() - no json type info..");
    return true;
  }

  if (status)
  {
    log_debug("LocalVariable::Setup() - create local AnyValue");
    ::ccs::base::SharedReference<const ::ccs::types::AnyType> const_type(local_type);
    _val = ::ccs::types::AnyValue(const_type);
    local_value.reset(new ::ccs::types::AnyValue(const_type));
  }

  if (status && HasAttribute(JSON_VALUE))
  {
    log_debug("LocalVariable::Setup() - parsing json value info..");
    std::string json_value = GetAttribute(JSON_VALUE);
    status = local_value->ParseInstance(json_value.c_str());
    if (status)
    {
      log_debug("LocalVariable::Setup() - copying parsed value..");
      status = SetValueImpl(*local_value);
    }
  }

  return status;  // empty AnyValue is allowed for setting.
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
