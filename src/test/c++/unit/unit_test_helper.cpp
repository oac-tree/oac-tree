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

#include "unit_test_helper.h"

#include <sup/sequencer/log.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/user_interface.h>

#include <common/BasicTypes.h>
#include <common/SysTools.h>

#include <cstdio>
#include <fstream>
#include <sstream>

namespace sup
{
namespace UnitTestHelper
{

static const char *ENV_TEST_RESOURCES_PATH_NAME = "TEST_RESOURCES_PATH";

const std::string CounterInstruction::Type = "Counter";
ccs::types::uint32 CounterInstruction::counter = 0u;

static bool _initialise_instruction =
    sup::sequencer::RegisterGlobalInstruction<CounterInstruction>();

ccs::types::uint32 CounterInstruction::GetCount(void)
{
  return counter;
}

sup::sequencer::ExecutionStatus CounterInstruction::ExecuteSingleImpl(
    sup::sequencer::UserInterface *ui, sup::sequencer::Workspace *ws)
{
  if (sup::sequencer::Instruction::HasAttribute("incr"))
  {
    ccs::types::uint32 incr =
        ccs::HelperTools::ToInteger(sup::sequencer::Instruction::GetAttribute("incr").c_str());
    counter += incr;
  }
  else
  {
    counter++;
  }

  return sup::sequencer::ExecutionStatus::SUCCESS;
}

CounterInstruction::CounterInstruction() : sup::sequencer::Instruction(CounterInstruction::Type)
{
  counter = 0u;
}
CounterInstruction::~CounterInstruction()
{
  counter = 0u;
}

void MockUI::UpdateInstructionStatusImpl(const sup::sequencer::Instruction *instruction) {}
int MockUI::GetUserChoiceImpl(const std::vector<std::string> &choices,
                              const std::string &description)
{
  sup::sequencer::log::Debug("TestUI::GetUserChoiceImpl - Description '%s'", description.c_str());
  return _choice;
}

bool MockUI::GetUserValueImpl(::ccs::types::AnyValue &value, const std::string &description)
{
  sup::sequencer::log::Debug("TestUI::GetUserValueImpl - Description '%s'", description.c_str());
  _type = value.GetType();
  value = _value;
  return _status;
}

::ccs::base::SharedReference<const ::ccs::types::AnyType> MockUI::GetType(void) const
{
  return _type;
}

void MockUI::SetChoice(int choice)
{
  _choice = choice;
}
void MockUI::SetStatus(bool status)
{
  _status = status;
}
void MockUI::SetValue(::ccs::types::AnyValue &value)
{
  _value = value;
}

MockUI::MockUI(void) {}
MockUI::~MockUI(void) {}

TemporaryTestFile::TemporaryTestFile(std::string filename_, std::string contents)
    : filename{filename_}
{
  std::ofstream file_out(filename);
  file_out.write(contents.c_str(), contents.size());
}

TemporaryTestFile::~TemporaryTestFile()
{
  std::remove(filename.c_str());
}

std::string GetFullTestFilePath(const std::string &filename)
{
  if (filename.empty() || filename[0] == '/')
  {
    return filename;
  }
  std::string resources_path(
      ::ccs::HelperTools::GetEnvironmentVariable(ENV_TEST_RESOURCES_PATH_NAME));
  if (resources_path.empty())
  {
    return filename;
  }
  if (resources_path[resources_path.size() - 1] != '/')
  {
    resources_path.push_back('/');
  }
  return resources_path + filename;
}

std::string CreateProcedureString(const std::string &body)
{
  static const std::string header{
      R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">)RAW"};

  static const std::string footer{R"RAW(</Procedure>)RAW"};

  return header + body + footer;
}

void PrintProcedureWorkspace(::sup::sequencer::Procedure *procedure)
{
  auto var_names = procedure->VariableNames();
  ::ccs::types::char8 val_string[1024];
  for (const auto &var_name : var_names)
  {
    ::ccs::types::AnyValue val;
    sup::sequencer::log::Debug("Variable '%s'", var_name.c_str());

    bool var_initialized = procedure->GetVariableValue(var_name, val);
    if (var_initialized)
    {
      val.SerialiseInstance(val_string, 1024);
      sup::sequencer::log::Debug("Variable '%s', with value\n  %s", var_name.c_str(), val_string);
    }
    else
    {
      sup::sequencer::log::Debug("Variable '%s' uninitialized", var_name.c_str());
    }
  }
}

std::string GetTextFileContent(const std::string &file_name)
{
  std::ifstream in(file_name);
  std::stringstream sstr;
  while(in >> sstr.rdbuf());
  return sstr.str();
}

}  // namespace UnitTestHelper

}  // namespace sup
