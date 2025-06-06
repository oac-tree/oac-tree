/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "unit_test_helper.h"

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/user_interface.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

//! Testing Output instruction.
class OutputTest : public ::testing::Test
{
public:
  //! Test interface which accepts values from the workspace.
  class TestInterface : public DefaultUserInterface
  {
  public:
    TestInterface(unsigned par) : m_value(sup::dto::UnsignedInteger32Type) { m_value = par; }

    bool PutValue(const sup::dto::AnyValue& value, const std::string& description) override
    {
      m_value = value;
      return true;
    }

    sup::dto::AnyValue m_value;
  };
};

TEST_F(OutputTest, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("Output");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("fromVar", "var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

// Takes integer value from the workspace and put in into TestInterface.

TEST_F(OutputTest, PutInteger)
{
  static const std::string procedure_body{
      R"RAW(
  <Sequence>
    <Output fromVar="var1"/>
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

TEST_F(OutputTest, VariableDescription)
{
  static const std::string procedure_body{
      R"RAW(
  <Sequence>
    <Output description="@descr" fromVar="var1"/>
  </Sequence>
  <Workspace>
    <Local name="descr" type='{"type":"string"}' value='"Hello"'/>
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

TEST_F(OutputTest, VariableDescriptionWrongType)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  static const std::string procedure_body{
      R"RAW(
  <Sequence>
    <Output description="@descr" fromVar="var1"/>
  </Sequence>
  <Workspace>
    <Local name="descr" type='{"type":"float32"}' value='4.3'/>
    <Local name="var1" type='{"type":"uint32"}' value='42' />
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);
  auto proc = ParseProcedureString(procedure_string);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(OutputTest, VariableDescriptionNotPresent)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  static const std::string procedure_body{
      R"RAW(
  <Sequence>
    <Output description="@descr" fromVar="var1"/>
  </Sequence>
  <Workspace>
    <Local name="var1" type='{"type":"uint32"}' value='42' />
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);
  auto proc = ParseProcedureString(procedure_string);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}
