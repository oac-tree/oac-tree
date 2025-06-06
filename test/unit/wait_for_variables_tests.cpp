/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : Ricardo Torres (EXT)
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

#include <gtest/gtest.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/log_severity.h>
#include <sup/oac-tree/sequence_parser.h>

using namespace sup::oac_tree;

const std::string kTestFileName = "test_file_variable.json";

class WaitForVariablesTest : public ::testing::Test
{
protected:
  WaitForVariablesTest();
  virtual ~WaitForVariablesTest();
private:
  sup::UnitTestHelper::TemporaryTestFile m_test_file;
};

TEST_F(WaitForVariablesTest, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("WaitForVariables");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
  EXPECT_TRUE(instr->AddAttribute("timeout", "1.0"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
  EXPECT_TRUE(instr->AddAttribute("varType", "Local"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST_F(WaitForVariablesTest, WaitForLocalVarsSuccess)
{
  const std::string body{
      R"(
    <Sequence>
        <WaitForVariables timeout="0.1" varType="Local"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='0' />
        <Local name="b" type='{"type":"uint8"}' value='1' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(WaitForVariablesTest, WaitForLocalVarsWithEmpty)
{
  const std::string body{
      R"(
    <Sequence>
        <WaitForVariables timeout="0.1" varType="Local"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='0' />
        <Local name="b" type='{"type":"uint8"}' value='1' />
        <Local name="c"/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(WaitForVariablesTest, WaitForFileVarsSuccess)
{
  const std::string body{
      R"(
    <Sequence>
        <Copy inputVar="a" outputVar="file"/>
        <WaitForVariables timeout="0.1" varType="File"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='0' />
        <File name="file" file="test_file_variable.json"/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(WaitForVariablesTest, WaitForFileVarsFailure)
{
  const std::string body{
      R"(
    <Sequence>
        <WaitForVariables timeout="0.1" varType="File"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='0' />
        <File name="file" file="does_not_exist.json"/>
    </Workspace>
)"};

  sup::UnitTestHelper::TestLogUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
  ASSERT_EQ(ui.m_log_entries.size(), 1);
  auto [severity, message] = ui.m_log_entries.front();
  EXPECT_EQ(severity, log::SUP_SEQ_LOG_WARNING);
  auto pos = message.find("file");
  EXPECT_NE(pos, std::string::npos);
}

TEST_F(WaitForVariablesTest, WaitForMultipleFileVarsFailure)
{
  const std::string body{
      R"(
    <Sequence>
        <WaitForVariables timeout="0.1" varType="File"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='0' />
        <File name="file1" file="does_not_exist.json"/>
        <File name="file2" file="does_not_exist.json"/>
    </Workspace>
)"};

  sup::UnitTestHelper::TestLogUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
  ASSERT_EQ(ui.m_log_entries.size(), 1);
  auto [severity, message] = ui.m_log_entries.front();
  EXPECT_EQ(severity, log::SUP_SEQ_LOG_WARNING);
  auto pos = message.find("file1");
  EXPECT_NE(pos, std::string::npos);
  pos = message.find("file2");
  EXPECT_NE(pos, std::string::npos);
}

WaitForVariablesTest::WaitForVariablesTest()
  : m_test_file{kTestFileName, ""}
{}

WaitForVariablesTest::~WaitForVariablesTest() = default;
