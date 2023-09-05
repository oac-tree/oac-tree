/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/user_interface.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

//! Testing Output instruction.
class OutputTest : public ::testing::Test
{
public:
  //! Test interface which accepts values from the workspace.
  class TestInterface : public UserInterface
  {
  public:
    TestInterface(unsigned par) : m_value(sup::dto::UnsignedInteger32Type) { m_value = par; }

    bool PutValue(const sup::dto::AnyValue& value, const std::string& description) override
    {
      m_value = value;
      return true;
    }

    bool GetUserValue(sup::dto::AnyValue&, const std::string&) override { return true; }

    void UpdateInstructionStatus(const Instruction* instruction) override {}

    void VariableUpdated(const std::string& name, const sup::dto::AnyValue& value,
                         bool connected) override
    {
    }

    int GetUserChoice(const std::vector<std::string>& options,
                      const sup::dto::AnyValue& metadata) override
    {
      return -1;
    }
    void Message(const std::string& message) override {}
    void Log(int severity, const std::string& message) override {}

    sup::dto::AnyValue m_value;
  };
};

TEST_F(OutputTest, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("Output");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("from", "var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

// Takes integer value from the workspace and put in into TestInterface.

TEST_F(OutputTest, PutInteger)
{
  static const std::string procedure_body{
      R"RAW(
  <Sequence>
    <Output from="var1"/>
  </Sequence>
  <Workspace>
    <Local name="var1" type='{"type":"uint32"}' value='42' />
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);

  const unsigned expected{22};
  TestInterface ui{expected};
  // initial state
  EXPECT_EQ(ui.m_value.As<unsigned>(), expected);

  auto proc = ParseProcedureString(procedure_string);
  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::SUCCESS));

  // checking that Output instruction has propagated the value to the interface
  EXPECT_EQ(ui.m_value.As<unsigned>(), 42);
}
