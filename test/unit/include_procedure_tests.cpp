/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : B.Bauvir (IO)
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
#include <sup/oac-tree/generic_utils.h>
#include <sup/oac-tree/sequence_parser.h>

#include <sup/xml/exceptions.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

const std::string kTestProcedureFileName_1 = "test_procedure_1.xml";

const std::string kTestProcedureBody_1{R"(
  <Sequence name="CopyAndCheck" isRoot="True">
      <Copy inputVar="a" outputVar="b"/>
      <Equals name="Check" leftVar="a" rightVar="b"/>
  </Sequence>
  <Wait name="ParameterizedWait" timeout="$timeout"/>
  <Inverter name="AlwaysFails">
      <Wait/>
  </Inverter>
  <Workspace>
      <Local name="a" type='{"type":"uint16"}' value='1' />
      <Local name="b" type='{"type":"uint16"}' value='0' />
  </Workspace>
)"};

const std::string kTestProcedureFileName_2 = "test_procedure_2.xml";

const std::string kTestProcedureBody_2{R"(
  <IncludeProcedure name="IncludeRoot" file="test_procedure_1.xml"/>
  <IncludeProcedure name="IncludeWait" file="test_procedure_1.xml" path="ParameterizedWait"/>
  <Workspace>
      <Local name="a" type='{"type":"string"}' value='"does_not_matter"' />
  </Workspace>
)"};

const std::string kTestProcedureFileName_3 = "test_procedure_3.xml";

const std::string kTestProcedureBody_3{R"(
  <RegisterType jsontype='{"type":"simple_struct_t","attributes":[{"a":{"type":"uint8"}},{"b":{"type":"uint8"}}]}'/>
  <Sequence name="CopyAndCheck" isRoot="True">
      <Copy inputVar="zero" outputVar="pair.a"/>
      <Copy inputVar="one" outputVar="pair.b"/>
      <Equals name="Check first" leftVar="pair.a" rightVar="zero"/>
      <Equals name="Check second" leftVar="pair.b" rightVar="one"/>
  </Sequence>
  <Workspace>
      <Local name="zero" type='{"type":"uint8"}' value='0' />
      <Local name="one" type='{"type":"uint8"}' value='1' />
      <Local name="pair" type='{"type":"simple_struct_t"}'/>
  </Workspace>
)"};

class IncludeProcedureTest : public ::testing::Test
{
protected:
  IncludeProcedureTest();
  virtual ~IncludeProcedureTest();
private:
  sup::UnitTestHelper::TemporaryTestFile m_test_file_1;
  sup::UnitTestHelper::TemporaryTestFile m_test_file_2;
  sup::UnitTestHelper::TemporaryTestFile m_test_file_3;
};

TEST_F(IncludeProcedureTest, Attributes)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("IncludeProcedure");
  ASSERT_NE(instr.get(), nullptr);

  // Missing file attribute
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  // Correct file attribute
  EXPECT_TRUE(instr->AddAttribute("file", "test_procedure_1.xml"));
  EXPECT_NO_THROW(instr->Setup(proc));

  // Wrong instruction path attribute
  EXPECT_TRUE(instr->AddAttribute("path", "does_not_exist"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
}

TEST_F(IncludeProcedureTest, WrongFile)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("IncludeProcedure");
  ASSERT_NE(instr.get(), nullptr);

  // Missing file attribute
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  // Wrong file attribute
  EXPECT_TRUE(instr->AddAttribute("file", "does_not_exist.xml"));
  EXPECT_THROW(instr->Setup(proc), sup::xml::ParseException);
}

TEST_F(IncludeProcedureTest, RootPath)
{
  const std::string body{R"(
    <IncludeProcedure file="test_procedure_1.xml"/>
    <Workspace/>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  sup::UnitTestHelper::EmptyUserInterface ui;
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(IncludeProcedureTest, TopInstruction)
{
  const std::string body{R"(
    <IncludeProcedure file="test_procedure_1.xml" path="AlwaysFails"/>
    <Workspace/>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  sup::UnitTestHelper::EmptyUserInterface ui;
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(IncludeProcedureTest, NestedInstruction)
{
  const std::string body{R"(
    <IncludeProcedure file="test_procedure_1.xml" path="CopyAndCheck.Check"/>
    <Workspace/>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  sup::UnitTestHelper::EmptyUserInterface ui;
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(IncludeProcedureTest, TwoLevelInclude)
{
  const std::string body{R"(
    <IncludeProcedure file="test_procedure_2.xml" path="IncludeRoot"/>
    <Workspace/>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  sup::UnitTestHelper::EmptyUserInterface ui;
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(IncludeProcedureTest, PlaceholderAttributes)
{
  const std::string body{R"(
    <IncludeProcedure file="test_procedure_2.xml" path="IncludeWait" timeout="0" />
    <Workspace/>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  sup::UnitTestHelper::EmptyUserInterface ui;
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(IncludeProcedureTest, TypeRegistrationInIncluded)
{
  const std::string body{R"(
    <IncludeProcedure file="test_procedure_3.xml"/>
    <Workspace/>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  sup::UnitTestHelper::EmptyUserInterface ui;
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

IncludeProcedureTest::IncludeProcedureTest()
  : m_test_file_1{kTestProcedureFileName_1,
                sup::UnitTestHelper::CreateProcedureString(kTestProcedureBody_1)}
  , m_test_file_2{kTestProcedureFileName_2,
                sup::UnitTestHelper::CreateProcedureString(kTestProcedureBody_2)}
  , m_test_file_3{kTestProcedureFileName_3,
                sup::UnitTestHelper::CreateProcedureString(kTestProcedureBody_3)}
{}

IncludeProcedureTest::~IncludeProcedureTest() = default;
