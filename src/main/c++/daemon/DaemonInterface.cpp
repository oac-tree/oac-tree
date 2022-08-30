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

#include "log.h"

// Local header files

#include "DaemonInterface.h"
#include "Instruction.h"

// Type definition

// Global variables

// Function declaration

namespace sup
{
namespace sequencer
{
// Function definition

void DaemonInterface::UpdateInstructionStatusImpl(const Instruction *instruction)
{
  if (_log_enabled)
  {
    auto instruction_type = instruction->GetType();
    auto instruction_name = instruction->GetName();
    auto status = instruction->GetStatus();

    log::Info("Instruction: (%s:%s) : %s", instruction_type.c_str(), instruction_name.c_str(),
             StatusToString(status).c_str());
  }
}

bool DaemonInterface::GetUserValueImpl(::ccs::types::AnyValue &, const std::string &)
{
  log::Warning("DaemonInterface::GetUserValueImpl() - not implemented");
  return false;
}

int DaemonInterface::GetUserChoiceImpl(const std::vector<std::string> &, const std::string &)
{
  log::Warning("DaemonInterface::GetUserChoiceImpl() - not implemented");
  return -1;
}

void DaemonInterface::StartSingleStepImpl()
{
  if (_log_enabled)
  {
    log::Info("Start single execution step");
  }
}

void DaemonInterface::EndSingleStepImpl()
{
  if (_log_enabled)
  {
    log::Info("End single execution step");
  }
}

void DaemonInterface::MessageImpl(const std::string& message)
{
  if (_log_enabled)
  {
    log::Info("%s", message.c_str());
  }
}

DaemonInterface::DaemonInterface(bool log_enabled) : _log_enabled{log_enabled} {}

DaemonInterface::~DaemonInterface() = default;

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C
