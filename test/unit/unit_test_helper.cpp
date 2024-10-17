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
 * Copyright (c) : 2010-2024 ITER Organization,
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
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/user_interface.h>

#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/json_value_parser.h>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>


namespace sup
{
namespace UnitTestHelper
{
using namespace sup::sequencer;

const std::string ENV_TEST_RESOURCES_PATH_NAME = "TEST_RESOURCES_PATH";

const std::string INCREMENT_ATTRIBUTE_NAME = "incr";

const std::string CounterInstruction::Type = "Counter";
unsigned long CounterInstruction::counter = 0u;

static bool _initialise_instruction = RegisterGlobalInstruction<CounterInstruction>();

const std::string TestTreeInstruction::Type = "TestTreeInstruction";

CounterInstruction::CounterInstruction()
  : Instruction(CounterInstruction::Type)
{
  AddAttributeDefinition(INCREMENT_ATTRIBUTE_NAME, sup::dto::UnsignedInteger64Type);
  counter = 0u;
}

CounterInstruction::~CounterInstruction()
{
  counter = 0u;
}

unsigned long CounterInstruction::GetCount()
{
  return counter;
}

ExecutionStatus CounterInstruction::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::uint64 increment = 1u;
  if (!GetAttributeValueAs(INCREMENT_ATTRIBUTE_NAME, ws, ui, increment))
  {
    return ExecutionStatus::FAILURE;
  }
  counter += increment;
  return ExecutionStatus::SUCCESS;
}

TestTreeInstruction::TestTreeInstruction()
  : CompoundInstruction(TestTreeInstruction::Type)
{}

TestTreeInstruction::~TestTreeInstruction() = default;

ExecutionStatus TestTreeInstruction::ExecuteSingleImpl(UserInterface&, Workspace&)
{
  return ExecutionStatus::SUCCESS;
}

std::unique_ptr<Instruction> CreateTestTreeInstruction(const std::string& name)
{
  std::unique_ptr<Instruction> result{new TestTreeInstruction{}};
  result->SetName(name);
  return result;
}

MockUI::MockUI()
  : m_status{false}
  , m_choice{-1}
  , m_value{}
  , m_options{}
  , m_metadata{}
{}

MockUI::~MockUI() = default;

sup::dto::AnyType MockUI::GetType() const
{
  return m_value.GetType();
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

std::vector<std::string> MockUI::GetOptions() const
{
  return m_options;
}

const sup::dto::AnyValue* MockUI::GetMetadata() const
{
  return m_metadata.get();
}

bool MockUI::GetUserValue(sup::dto::AnyValue &value, const std::string &description)
{
  if (sup::dto::IsEmptyValue(value))
  {
    return false;
  }
  if (value.GetType() != m_value.GetType())
  {
    return false;
  }
  value = m_value;
  return m_status;
}

int MockUI::GetUserChoice(const std::vector<std::string>& options,
                              const sup::dto::AnyValue& metadata)
{
  m_options = options;
  m_metadata.reset(new sup::dto::AnyValue{metadata});
  return m_choice;
}

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
    utils::GetEnvironmentVariable(ENV_TEST_RESOURCES_PATH_NAME);
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

void PrintProcedureWorkspace(Procedure *procedure)
{
  auto var_names = procedure->VariableNames();
  sup::dto::char8 val_string[1024];
  for (const auto &var_name : var_names)
  {
    sup::dto::AnyValue val;

    bool var_initialized = procedure->GetVariableValue(var_name, val);
    if (var_initialized)
    {
      std::string json_rep = sup::dto::ValuesToJSONString(val);
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
