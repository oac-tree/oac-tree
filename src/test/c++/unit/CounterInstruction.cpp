/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project       : CODAC Supervision and Automation (SUP) Sequencer component
*
* Description   : Instruction node implementation
*
* Author        : B.Bauvir (IO)
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

#include <common/BasicTypes.h> // Misc. type definition

#include <common/log-api.h> // Syslog wrapper routines

#include <common/AnyValue.h>
#include <common/AnyValueHelper.h>

#include <Instruction.h>
#include <InstructionRegistry.h>

// Local header files

#include "CounterInstruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Function declaration

// Global variables

const std::string CounterInstruction::Type = "Counter";
ccs::types::uint32 CounterInstruction::counter = 0u;

static bool _initialise_instruction = RegisterGlobalInstruction<CounterInstruction>();

// Function definition

ccs::types::uint32 CounterInstruction::GetCount (void) { return counter; }

ExecutionStatus CounterInstruction::ExecuteSingleImpl (sup::sequencer::UserInterface * ui, sup::sequencer::Workspace * ws)
{ 

  if (Instruction::HasAttribute("incr"))
    {
      ccs::types::uint32 incr = ccs::HelperTools::ToInteger(Instruction::GetAttribute("incr").c_str());
      counter += incr;
    }
  else
    {
      counter++; 
    }

  return ExecutionStatus::SUCCESS; 

}

CounterInstruction::CounterInstruction() : Instruction(CounterInstruction::Type) { counter = 0u; }
CounterInstruction::~CounterInstruction() { counter = 0u; }

} // namespace sequencer

} // namespace sup

#undef LOG_ALTERN_SRC
