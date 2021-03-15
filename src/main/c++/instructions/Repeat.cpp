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

#include "Repeat.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

const std::string Repeat::Type = "Repeat";

// Function declaration

// Function definition

void Repeat::InitHook()
{
  _count = 0;
}

ExecutionStatus Repeat::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
  if (!_child || _max_count == 0)
  {
    return ExecutionStatus::SUCCESS;
  }

  auto child_status = _child->GetStatus();

  if (child_status == ExecutionStatus::SUCCESS)
  {
    _child->Reset();
  }

  _child->ExecuteSingle(ui, ws);

  child_status = _child->GetStatus();
  // Don't increment count when _max_count is not strictly positive.
  if (_max_count > 0 && (child_status == ExecutionStatus::SUCCESS ||
                         child_status == ExecutionStatus::FAILURE))
  {
    _count++;
  }

  return CalculateStatus();
}

ExecutionStatus Repeat::CalculateStatus() const
{
  auto child_status = _child->GetStatus();

  if (child_status == ExecutionStatus::SUCCESS)
  {
    if (_count == _max_count)
    {
      return ExecutionStatus::SUCCESS;
    }
    else
    {
      return ExecutionStatus::NOT_FINISHED;
    }
  }

  return child_status;
}

bool Repeat::SetupImpl(const Procedure & proc)
{
  bool status = HasAttribute("maxCount");
  if (status)
  {
    auto max_str = GetAttribute("maxCount");
    try
    {
      _max_count = std::stoi(max_str);
      if (_max_count < 0)
      {
        _max_count = -1;
      }
    }
    catch(const std::exception&)
    {
      log_warning("Repeat::InitMaxCount() - could not parse maxCount attribute!");
      status = false;
    }
  }
  return DecoratorInstruction::SetupImpl(proc) && status;
}

Repeat::Repeat()
  : DecoratorInstruction(Type)
  , _max_count{0}
  , _count{0}
  , _init_ok{false}
{}

Repeat::~Repeat()
{}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
