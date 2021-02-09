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
#include <common/AnyTypeHelper.h>
#include <common/AnyValueHelper.h>

// Local header files

#include "VariableParser.h"
#include "LocalVariable.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

std::unique_ptr<Variable> ParseVariable(const TreeData & data)
{
  std::unique_ptr<Variable> result;
  bool status = data.HasAttribute(LOCAL_VARIABLE_JSON_TYPE);

  ::ccs::base::SharedReference<::ccs::types::AnyType> local_type;
  std::unique_ptr<::ccs::types::AnyValue> local_value;
  if (status)
  {
    log_info("sup::sequencer::ParseVariable() - parsing json type info..");
    std::string json_type = data.GetAttribute(LOCAL_VARIABLE_JSON_TYPE);
    auto read = ::ccs::HelperTools::Parse(local_type, json_type.c_str());
    status = read > 0;
  }
  else
  {
    log_warning("sup::sequencer::ParseVariable() - no json type info!");
  }

  if (status)
  {
    log_info("sup::sequencer::ParseVariable() - create LocalVariable..");
    ::ccs::base::SharedReference<const ::ccs::types::AnyType> const_type(local_type);
    result.reset(new LocalVariable(const_type));
    local_value.reset(new ::ccs::types::AnyValue(const_type));
    status = data.HasAttribute(LOCAL_VARIABLE_JSON_VALUE);
  }

  if (status)
  {
    log_info("sup::sequencer:ParseVariable() - parsing json value info..");
    std::string json_value = data.GetAttribute(LOCAL_VARIABLE_JSON_VALUE);
    status = local_value->ParseInstance(json_value.c_str());
  }

  if (status)
  {
    log_info("sup::sequencer:ParseVariable() - copying parsed value..");
    result->SetValue(*local_value);
  }

  return result;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
