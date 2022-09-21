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

#include <sup/sequencer/generic_utils.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/log.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/user_interface.h>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

namespace sup
{
namespace UnitTestHelper
{

static const std::string ENV_TEST_RESOURCES_PATH_NAME = "TEST_RESOURCES_PATH";

const std::string CounterInstruction::Type = "Counter";
unsigned CounterInstruction::counter = 0u;

static bool _initialise_instruction =
    sup::sequencer::RegisterGlobalInstruction<CounterInstruction>();

unsigned CounterInstruction::GetCount()
{
  return counter;
}

sup::sequencer::ExecutionStatus CounterInstruction::ExecuteSingleImpl(
    sup::sequencer::UserInterface *ui, sup::sequencer::Workspace *ws)
{
  if (sup::sequencer::Instruction::HasAttribute("incr"))
  {
    unsigned incr = sup::sequencer::utils::StringToUnsigned(
      sup::sequencer::Instruction::GetAttribute("incr"));
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
  return m_choice;
}

bool MockUI::GetUserValueImpl(sup::dto::AnyValue &value, const std::string &description)
{
  sup::sequencer::log::Debug("TestUI::GetUserValueImpl - Description '%s'", description.c_str());
  m_type = value.GetType();
  if (sup::dto::IsEmptyType(m_type))
  {
    return false;
  }
  try
  {
    value = m_value;
  }
  catch(const sup::dto::InvalidConversionException&)
  {
    return false;
  }
  return m_status;
}

sup::dto::AnyType MockUI::GetType() const
{
  return m_type;
}

void MockUI::SetChoice(int choice)
{
  m_choice = choice;
}
void MockUI::SetStatus(bool status)
{
  m_status = status;
}
void MockUI::SetValue(sup::dto::AnyValue &value)
{
  m_value = value;
}

MockUI::MockUI() {}
MockUI::~MockUI() {}

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
  std::string resources_path =
    sup::sequencer::utils::GetEnvironmentVariable(ENV_TEST_RESOURCES_PATH_NAME);
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

void PrintProcedureWorkspace(sup::sequencer::Procedure *procedure)
{
  auto var_names = procedure->VariableNames();
  sup::dto::char8 val_string[1024];
  for (const auto &var_name : var_names)
  {
    sup::dto::AnyValue val;
    sup::sequencer::log::Debug("Variable '%s'", var_name.c_str());

    bool var_initialized = procedure->GetVariableValue(var_name, val);
    if (var_initialized)
    {
      std::string json_rep = sup::dto::ValuesToJSONString(val);
      sup::sequencer::log::Debug("Variable '%s', with value\n  %s", var_name.c_str(),
                                 json_rep.c_str());
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
