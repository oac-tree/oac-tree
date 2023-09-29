/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : B.Bauvir (IO)
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
#include <sup/sequencer/generic_utils.h>
#include <sup/sequencer/sequence_parser.h>

#include <sup/xml/exceptions.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

const std::string kTestProcedureFileName_1 = "test_procedure_1.xml";

const std::string kTestProcedureBody_1{R"(
  <Sequence name="CopyAndCheck" isRoot="True">
      <Copy inputVar="a" outputVar="b"/>
      <Equals name="Check" lhs="a" rhs="b"/>
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

class IncludeProcedureTest : public ::testing::Test
{
protected:
  IncludeProcedureTest();
  virtual ~IncludeProcedureTest();
private:
  sup::UnitTestHelper::TemporaryTestFile m_test_file_1;
  sup::UnitTestHelper::TemporaryTestFile m_test_file_2;
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

TEST_F(IncludeProcedureTest,PlaceholderAttributes)
{
  const std::string body{R"(
    <IncludeProcedure file="test_procedure_2.xml" path="IncludeWait" timeout="0" />
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
{}

IncludeProcedureTest::~IncludeProcedureTest() = default;