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
* Copyright (c) : 2010-2021 ITER Organization,
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
#include <map>
#include <memory>

#include <common/AnyValueHelper.h>
#include <common/log-api.h>

// Local header files

#include "DaemonInterface.h"
#include "Instruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

// Global variables

// Function declaration

namespace sup {

namespace sequencer {

// Function definition

void DaemonInterface::UpdateInstructionStatusImpl(const Instruction * instruction)
{
  if (_log_enabled)
  {
    auto instruction_type = instruction->GetType();
    auto instruction_name = instruction->GetName();
    auto status = instruction->GetStatus();

    log_info("Instruction: (%s:%s) : %s", instruction_type.c_str(), instruction_name.c_str(),
                                          StatusToString(status).c_str());
  }
}

bool DaemonInterface::GetUserValueImpl(::ccs::types::AnyValue &, const std::string &)
{
  log_warning("DaemonInterface::GetUserValueImpl() - not implemented");
  return false;
}

int DaemonInterface::GetUserChoiceImpl(const std::vector<std::string> &, const std::string &)
{
  log_warning("DaemonInterface::GetUserChoiceImpl() - not implemented");
  return -1;
}

void DaemonInterface::StartSingleStepImpl()
{
  if (_log_enabled)
  {
    log_info("Start single execution step");
  }
}

void DaemonInterface::EndSingleStepImpl()
{
  if (_log_enabled)
  {
    log_info("End single execution step");
  }
}

DaemonInterface::DaemonInterface(bool log_enabled)
  : _log_enabled{log_enabled}
{}

DaemonInterface::~DaemonInterface() = default;

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
