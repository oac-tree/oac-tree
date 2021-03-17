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

#include <iostream>
#include <sstream>

#include <common/AnyValueHelper.h>
#include <common/log-api.h>

// Local header files

#include "CLInterface.h"
#include "Instruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

// Global variables

// Function declaration

static bool ParseStringToScalarAnyvalue(::ccs::types::AnyValue & value, const std::string & str);

namespace sup {

namespace sequencer {

// Function definition

void CLInterface::UpdateInstructionStatusImpl(const Instruction * instruction)
{
  auto instruction_type = instruction->GetType();
  auto instruction_name = instruction->GetName();
  auto status = instruction->GetStatus();

  std::cout << "Instruction: (" << instruction_type << ":" << instruction_name << ") : ";
  std::cout << StatusToString(status) << std::endl;
}

bool CLInterface::GetUserValueImpl(::ccs::types::AnyValue & value, const std::string & description)
{
  if (! ::ccs::HelperTools::Is<::ccs::types::ScalarType>(&value))
  {
    log_warning("CLInterface::GetUserValueImpl(value, '%s') only supports scalar values..",
                description.c_str());
    return false;
  }
  std::cout << description << " (" << value.GetType()->GetName() << "): ";
  std::string input;
  std::getline(std::cin, input);
  bool result = ParseStringToScalarAnyvalue(value, input);
  if (!result)
  {
    log_warning("CLInterface::GetUserValueImpl(value, '%s') could not parse '%s' into value..",
                input.c_str());
  }
  return result;
}

int CLInterface::GetUserChoiceImpl(const std::vector<std::string> & choices,
                                   const std::string & description)
{
  std::cout << "Select one of the following options (" << description << "):" << std::endl;
  for (int i=0; i<choices.size(); ++i)
  {
    std::cout << i << ": " << choices[i] << std::endl;
  }
  int input = -1;
  std::cin >> input;
  if (std::cin.fail() || input < 0 || input >=choices.size())
  {
    log_warning("CLInterface::GetUserChoiceImpl() - invalid choice");
    return -1;
  }
  std::cout << choices[input] << " selected" << std::endl;
  return input;
}


void CLInterface::StartSingleStepImpl()
{
  if (_verbose)
  {
    std::cout << "Start single execution step" << std::endl;
  }
}

void CLInterface::EndSingleStepImpl()
{
  if (_verbose)
  {
    std::cout << "End single execution step" << std::endl;
  }
}

CLInterface::CLInterface(bool verbose)
  : _verbose{verbose}
{}

CLInterface::~CLInterface() = default;

} // namespace sequencer

} // namespace sup

template<typename T>
bool ParseStringTo(::ccs::types::AnyValue & value, const std::string & str)
{
  std::istringstream istr(str);
  T val;
  istr >> val;
  if (istr.fail())
  {
    log_warning("ParseStringToScalarAnyvalue() - could not parse ('%s') in type ('%s)",
                str.c_str(), value.GetType()->GetName());
    return false;
  }
  value = val;
  return true;
}

static bool ParseStringToScalarAnyvalue(::ccs::types::AnyValue & value, const std::string & str)
{
  std::string type_name = value.GetType()->GetName();
  if (type_name == "bool")
  {
    return ParseStringTo<::ccs::types::boolean>(value, str);
  }
  else if (type_name == "char8")
  {
    return ParseStringTo<::ccs::types::char8>(value, str);
  }
  else if (type_name == "int8")
  {
    return ParseStringTo<::ccs::types::int8>(value, str);
  }
  else if (type_name == "uint8")
  {
    return ParseStringTo<::ccs::types::uint8>(value, str);
  }
  else if (type_name == "int16")
  {
    return ParseStringTo<::ccs::types::int16>(value, str);
  }
  else if (type_name == "uint16")
  {
    return ParseStringTo<::ccs::types::uint16>(value, str);
  }
  else if (type_name == "int32")
  {
    return ParseStringTo<::ccs::types::int32>(value, str);
  }
  else if (type_name == "uint32")
  {
    return ParseStringTo<::ccs::types::uint32>(value, str);
  }
  else if (type_name == "int64")
  {
    return ParseStringTo<::ccs::types::int64>(value, str);
  }
  else if (type_name == "uint64")
  {
    return ParseStringTo<::ccs::types::uint64>(value, str);
  }
  else if (type_name == "float32")
  {
    return ParseStringTo<::ccs::types::float32>(value, str);
  }
  else if (type_name == "float64")
  {
    return ParseStringTo<::ccs::types::float64>(value, str);
  }
  else if (type_name == "string")
  {
    ::ccs::types::string buffer;
    ::ccs::HelperTools::SafeStringCopy(buffer, str.c_str(), 64);
    value = buffer;
    return true;
  }
  return false;
}

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
