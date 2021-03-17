/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project       : CODAC Supervision and Automation (SUP) Sequencer component
*
* Description   : File-based variable implementation
*
* Author        : Bertrand Bauvir (IO)
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

#include <common/AnyValueHelper.h> // Misc. helper functions

// Local header files

#include "FileVariable.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Function declaration

// Global variables

const std::string FileVariable::Type = "FileVariable";

// Function definition

bool FileVariable::SetupImpl (void)
{
  bool status = Variable::HasAttribute("file");
  return status;
}

bool FileVariable::GetValueImpl (::ccs::types::AnyValue& value) const
{
  bool status = ::ccs::HelperTools::ReadFromFile(&value, Variable::GetAttribute("file").c_str());
  return status;
}

bool FileVariable::SetValueImpl (const ::ccs::types::AnyValue& value)
{
  bool status = static_cast<bool>(value.GetType()); // Valid variable
  if (status)
  {
    status = ::ccs::HelperTools::DumpToFile(&value, Variable::GetAttribute("file").c_str());
  }
  return status;
}

FileVariable::FileVariable (void) : Variable(FileVariable::Type) {}
FileVariable::~FileVariable (void) {}

} // namespace sequencer

} // namespace sup

#undef LOG_ALTERN_SRC
