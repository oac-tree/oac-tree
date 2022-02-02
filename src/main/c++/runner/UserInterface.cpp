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

// Global header files

#include <common/log-api.h>

// Local header files

#include "UserInterface.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup
{
namespace sequencer
{
// Global variables

// Function declaration

// Function definition

void UserInterface::VariableUpdatedImpl(const std::string& name, const ccs::types::AnyValue& value)
{}

bool UserInterface::PutValueImpl(const ::ccs::types::AnyValue& value,
                                 const std::string& description)
{
  return false;
}

bool UserInterface::GetUserValueImpl(::ccs::types::AnyValue& value, const std::string& description)
{
  return false;
}

int UserInterface::GetUserChoiceImpl(const std::vector<std::string>& choices,
                                     const std::string& description)
{
  return -1;
}

void UserInterface::StartSingleStepImpl() {}

void UserInterface::EndSingleStepImpl() {}

void UserInterface::MessageImpl(const std::string&) {}

void UserInterface::UpdateInstructionStatus(const Instruction* instruction)
{
  std::lock_guard<std::mutex> lock(_ui_mutex);
  UpdateInstructionStatusImpl(instruction);
}

void UserInterface::VariableUpdated(const std::string& name, const ccs::types::AnyValue& value)
{
  VariableUpdatedImpl(name, value);
}

bool UserInterface::PutValue(const ::ccs::types::AnyValue& value, const std::string& description)
{
  std::lock_guard<std::mutex> lock(_ui_mutex);
  return PutValueImpl(value, description);
}

bool UserInterface::GetUserValue(::ccs::types::AnyValue& value, const std::string& description)
{
  std::lock_guard<std::mutex> lock(_ui_mutex);
  return GetUserValueImpl(value, description);
}

int UserInterface::GetUserChoice(const std::vector<std::string>& choices,
                                 const std::string& description)
{
  std::lock_guard<std::mutex> lock(_ui_mutex);
  return GetUserChoiceImpl(choices, description);
}

void UserInterface::StartSingleStep()
{
  std::lock_guard<std::mutex> lock(_ui_mutex);
  StartSingleStepImpl();
}

void UserInterface::EndSingleStep()
{
  std::lock_guard<std::mutex> lock(_ui_mutex);
  EndSingleStepImpl();
}

void UserInterface::Message(const std::string& message)
{
  std::lock_guard<std::mutex> lock(_ui_mutex);
  MessageImpl(message);
}

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C

#undef LOG_ALTERN_SRC
