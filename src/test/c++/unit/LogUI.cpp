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

#include "Instruction.h"
#include "LogUI.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer::LogUI"

// Type definition

namespace sup
{
namespace sequencer
{
// Global variables

// Function declaration

// Function definition

void LogUI::UpdateInstructionStatusImpl(const Instruction* instruction)
{
  auto instruction_type = instruction->GetType();
  auto status = StatusToString(instruction->GetStatus());

  log_debug("Instruction '%s', status '%s'", instruction_type.c_str(), status.c_str());
}

void LogUI::StartSingleStepImpl()
{
  log_debug("Start single execution step");
}

void LogUI::EndSingleStepImpl()
{
  log_debug("End single execution step");
}

LogUI::LogUI() = default;

LogUI::~LogUI() = default;

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C

#undef LOG_ALTERN_SRC
