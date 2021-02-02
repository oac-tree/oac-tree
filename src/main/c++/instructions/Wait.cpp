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

#include "Wait.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

const std::string Wait::Type = "Wait";

// Function declaration

// Function definition

void Wait::InitHook()
{
  if (HasAttribute("timeout"))
  {
    auto timeout = GetAttribute("timeout");
    try
    {
      double t = std::stod(timeout);
      if (t > 0.0)
      {
        _timeout = t;
      }
    }
    catch(const std::exception&)
    {
      log_warning("Wait::InitHook() - could not parse timeout attribute!");
    }
  }
}

ExecutionStatus Wait::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
    (void)ui;
    (void)ws;
    if (_timeout > 0.0)
    {
      auto mseconds = static_cast<int>(_timeout * 1000);
      std::this_thread::sleep_for(std::chrono::milliseconds(mseconds));
    }
    return ExecutionStatus::SUCCESS;
}

Wait::Wait()
    : Instruction(Type)
    , _timeout(0.0)
{}

Wait::~Wait()
{}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
