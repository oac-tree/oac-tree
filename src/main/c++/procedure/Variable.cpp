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

#include "Variable.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

Variable::~Variable() =default;

bool Variable::GetValue(::ccs::types::AnyValue& value) const
{
  std::lock_guard<std::mutex> lock(_access_mutex);
  return GetValueImpl(value);
}

bool Variable::SetValue(const ::ccs::types::AnyValue& value)
{
  std::lock_guard<std::mutex> lock(_access_mutex);
  return SetValueImpl(value);
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
