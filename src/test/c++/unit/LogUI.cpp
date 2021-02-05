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

#include "LogUI.h"
#include "Instruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer::LogUI"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

LogUI::LogUI() = default;

LogUI::~LogUI() = default;

void LogUI::UpdateInstructionStatus(const Instruction * instruction)
{
    auto instruction_type = instruction->GetType();
    auto status = StatusToString(instruction->GetStatus());

    log_info("Instruction '%s', status '%s'",
             instruction_type.c_str(), status.c_str());

}

void LogUI::StartSingleStep()
{
    log_info("Start single execution step");
}

void LogUI::EndSingleStep()
{
    log_info("End single execution step");
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
