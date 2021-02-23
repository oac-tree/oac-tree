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
  InitMaxCount();
  _count = 0;
}

ExecutionStatus Repeat::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
  if (!_child || _max_count == 0)
  {
    return ExecutionStatus::SUCCESS;
  }

  if (!_init_ok)
  {
    return ExecutionStatus::FAILURE;
  }

  auto child_status = _child->GetStatus();

  if (child_status == ExecutionStatus::SUCCESS)
  {
    _child->ResetStatus();
  }
  auto child_type=_child->GetType();
  auto child_name= _child->GetName();
  log_info("Repeat::ExecuteSingleImp - Executing child %s:%s", child_type.c_str(), child_name.c_str() );
  _child->ExecuteSingle(ui, ws);
  log_info("Repeat::ExecuteSingleImp - Executed child %s:%s", child_type.c_str(), child_name.c_str() );

  child_status = _child->GetStatus();
  // Don't increment count when _max_count is not strictly positive.
  if (_max_count > 0 && (child_status == ExecutionStatus::SUCCESS ||
                         child_status == ExecutionStatus::FAILURE))
  {
    _count++;
  }

  return CalculateStatus();
}

void Repeat::ResetHook()
{
  if (_child)
  {
    _child->ResetStatus();
  }
  _init_ok = false;
  _count = 0;
}

void Repeat::InitMaxCount()
{
  bool status = HasAttribute("maxCount");
  _max_count = -1;

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
  _init_ok = status;
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

Repeat::Repeat()
    : DecoratorInstruction(Type)
{}

Repeat::~Repeat()
{}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
