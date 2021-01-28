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

#include <chrono>
#include <thread>

// Local header files

#include "Runner.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

Runner::Runner(UserInterface * ui)
  : _proc{nullptr}
  , _ui{ui}
{}

Runner::~Runner()
{}

void Runner::SetProcedure(Procedure * procedure)
{
  _proc = procedure;
}

void Runner::ExecuteProcedure()
{
  if (_proc)
  {
    while(!IsFinished())
    {
      ExecuteSingle();
      if (IsRunning())
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    }
  }
}

void Runner::ExecuteSingle()
{
  if (_proc)
  {
    _ui->StartSingleStep();
    _proc->ExecuteSingle(_ui);
    _ui->EndSingleStep();
  }
}

bool Runner::IsFinished() const
{
  if (!_proc)
  {
    return true;
  }

  auto status = _proc->GetStatus();
  return (status == ExecutionStatus::SUCCESS ||
          status == ExecutionStatus::FAILURE);
}

bool Runner::IsRunning() const
{
  if (!_proc)
  {
    return false;
  }

  auto status = _proc->GetStatus();
  return (status == ExecutionStatus::RUNNING);
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
