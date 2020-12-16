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

#include "RepeatNode.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

const std::string RepeatNode::Type = "RepeatNode";

// Function declaration

// Function definition

void RepeatNode::InitHook()
{
  InitMaxCount();
  _count = 0;
}

ExecutionStatus RepeatNode::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
  if (!_child)
  {
    return ExecutionStatus::SUCCESS;
  }

  if (!_init_ok)
  {
    return ExecutionStatus::FAILURE;
  }

  auto child_status = _child->GetStatus();

  if (child_status == ExecutionStatus::RUNNING)
  {
    return ExecutionStatus::RUNNING;
  }

  if (child_status == ExecutionStatus::SUCCESS)
  {
    _child->ResetStatus();
  }

  if (_count < _max_count || _max_count == -1)
  {
      _child->ExecuteSingle(ui, ws);
      _count++;
  }

  return CalculateStatus();
}

void RepeatNode::ResetHook()
{
  _init_ok = false;
  _count = 0;
}

void RepeatNode::InitMaxCount()
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
      log_warning("RepeatNode::InitMaxCount() - could not parse maxCount attribute!");
      status = false;
    }
  }
  _init_ok = status;
}

ExecutionStatus RepeatNode::CalculateStatus() const
{
  auto child_status = _child->GetStatus();

  if (_count == _max_count && child_status == ExecutionStatus::SUCCESS)
  {
    return ExecutionStatus::SUCCESS;
  }

  if (child_status == ExecutionStatus::UNDEFINED ||
      child_status == ExecutionStatus::STARTED ||
      child_status == ExecutionStatus::SUCCESS)
  {
    return GetStatus();
  }

  return child_status;
}

RepeatNode::RepeatNode()
    : DecoratorInstruction(Type)
{}

RepeatNode::~RepeatNode()
{}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
