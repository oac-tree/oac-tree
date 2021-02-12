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

// Function declaration

// Function definition

LocalVariable::LocalVariable()
  : _val{}
  , _initialized{false}
{}

LocalVariable::LocalVariable(const ::ccs::base::SharedReference<const ccs::types::AnyType>& type)
  : _val{type}
  , _initialized{false}
{
  log_info("sup::sequencer::LocalVariable::LocalVariable('%s') - create LocalVariable..",
           type->GetName());
}

LocalVariable::~LocalVariable() {}

bool LocalVariable::GetValueImpl(::ccs::types::AnyValue& value) const
{
  if (!_initialized)
  {
    log_warning("sup::sequencer::LocalVariable::GetValue() - not initialized..");
    return false;
  }

  if (!value.GetType() || value.GetSize() == _val.GetSize())
  {
    log_info("sup::sequencer::LocalVariable::GetValue() - copying value..");
    value = _val;
    return true;
  }
  log_warning("sup::sequencer::LocalVariable::GetValue() - incompatible types..");
  return false;
}

bool LocalVariable::SetValueImpl(const ::ccs::types::AnyValue& value)
{
  if (_val.GetSize() == value.GetSize())
  {
    log_info("sup::sequencer::LocalVariable::SetValue() - copying value..");
    _val = value;
    _initialized = true;
    return true;
  }
  log_warning("sup::sequencer::LocalVariable::SetValue() - incompatible types..");
  return false;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
