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

#include <common/SysTools.h>
#include <common/log-api.h>

#include <common/BasicTypes.h>

// Local header files

#include "Instruction.h"
#include "InstructionRegistry.h"
#include "UserInterface.h"

#include "UnitTestHelper.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

namespace sup {

namespace UnitTestHelper {

// Function declaration

// Global variables

static const char * ENV_TEST_RESOURCES_PATH_NAME = "TEST_RESOURCES_PATH";

const std::string CounterInstruction::Type = "Counter";
ccs::types::uint32 CounterInstruction::counter = 0u;

static bool _initialise_instruction = sup::sequencer::RegisterGlobalInstruction<CounterInstruction>();

// Function definition

ccs::types::uint32 CounterInstruction::GetCount (void) { return counter; }

sup::sequencer::ExecutionStatus CounterInstruction::ExecuteSingleImpl (sup::sequencer::UserInterface * ui, sup::sequencer::Workspace * ws)
{ 

  if (sup::sequencer::Instruction::HasAttribute("incr"))
    {
      ccs::types::uint32 incr = ccs::HelperTools::ToInteger(sup::sequencer::Instruction::GetAttribute("incr").c_str());
      counter += incr;
    }
  else
    {
      counter++; 
    }

  return sup::sequencer::ExecutionStatus::SUCCESS; 

}

CounterInstruction::CounterInstruction() : sup::sequencer::Instruction(CounterInstruction::Type) { counter = 0u; }
CounterInstruction::~CounterInstruction() { counter = 0u; }

void MockUI::UpdateInstructionStatusImpl (const sup::sequencer::Instruction * instruction) {}
int MockUI::GetUserChoiceImpl (const std::vector<std::string> & choices, const std::string & description)
{
  log_info("TestUI::GetUserChoiceImpl - Description '%s'", description.c_str());
  return _choice; 
}

bool MockUI::GetUserValueImpl (::ccs::types::AnyValue & value, const std::string & description)
{
  log_info("TestUI::GetUserValueImpl - Description '%s'", description.c_str());
  _type = value.GetType();
  value = _value;
  return _status;
}

::ccs::base::SharedReference<const ::ccs::types::AnyType> MockUI::GetType (void) const { return _type; }

void MockUI::SetChoice (int choice) { _choice = choice; }
void MockUI::SetStatus (bool status) { _status = status; }
void MockUI::SetValue (::ccs::types::AnyValue & value) { _value = value; }

MockUI::MockUI (void) {}
MockUI::~MockUI (void) {}

std::string GetFullTestFilePath(const std::string & filename)
{
  if (filename.empty() || filename[0] == '/')
  {
    return filename;
  }
  std::string resources_path(::ccs::HelperTools::GetEnvironmentVariable(
      ENV_TEST_RESOURCES_PATH_NAME));
  if (resources_path.empty())
  {
    return filename;
  }
  if (resources_path[resources_path.size()-1] != '/')
  {
    resources_path.push_back('/');
  }
  return resources_path + filename;
}

} // namespace UnitTestHelper

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
