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

#include "LogUI.h"

#include <sup/sequencer/log.h>
#include <sup/sequencer/instruction.h>

namespace sup
{
namespace sequencer
{
void LogUI::UpdateInstructionStatusImpl(const Instruction* instruction)
{
  auto instruction_type = instruction->GetType();
  auto status = StatusToString(instruction->GetStatus());

  log::Debug("Instruction '%s', status '%s'", instruction_type.c_str(), status.c_str());
}

void LogUI::StartSingleStepImpl()
{
  log::Debug("Start single execution step");
}

void LogUI::EndSingleStepImpl()
{
  log::Debug("End single execution step");
}

LogUI::LogUI() = default;

LogUI::~LogUI() = default;

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C
