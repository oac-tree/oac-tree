/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/oac-tree/procedure.h>

#include "mock_user_interface.h"
#include "unit_test_helper.h"

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/log_severity.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

class VarExistTest : public ::testing::Test
{
protected:
  VarExistTest();
  virtual ~VarExistTest();

  MockUserInterface mock_ui;
};

TEST_F(VarExistTest, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("VarExists");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("varName", "does_not_matter"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST_F(VarExistTest, EmptyVarName)
{
  const std::string body{R"(
    <VarExists varName=""/>
    <Workspace>
    </Workspace>
)"};

  sup::UnitTestHelper::TestLogUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
  ASSERT_EQ(ui.m_log_entries.size(), 1);
  EXPECT_EQ(ui.m_log_entries[0].first, log::SUP_SEQ_LOG_ERR);
}

TEST_F(VarExistTest, SuccessNoField)
{
  const std::string body{R"(
    <VarExists varName="myfloat"/>
    <Workspace>
        <Local name="myfloat" type='{"type":"float32"}' />
    </Workspace>
)"};

  sup::UnitTestHelper::TestLogUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
  EXPECT_EQ(ui.m_log_entries.size(), 0);
}

TEST_F(VarExistTest, SuccessField)
{
  const std::string body{R"(
    <VarExists varName="mystruct.value"/>
    <Workspace>
        <Local name="mystruct" type='{"type":"MyStruct","attributes":[{"value":{"type":"float32"}}]}' />
    </Workspace>
)"};

  sup::UnitTestHelper::TestLogUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
  EXPECT_EQ(ui.m_log_entries.size(), 0);
}

TEST_F(VarExistTest, FailureNoField)
{
  const std::string body{R"(
    <VarExists varName="myfloat"/>
    <Workspace>
    </Workspace>
)"};

  sup::UnitTestHelper::TestLogUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
  EXPECT_NE(ui.m_log_entries.size(), 0);
}

TEST_F(VarExistTest, FailureField)
{
  const std::string body{R"(
    <VarExists varName="mystruct.val"/>
    <Workspace>
        <Local name="mystruct" type='{"type":"MyStruct","attributes":[{"value":{"type":"float32"}}]}' />
    </Workspace>
)"};

  sup::UnitTestHelper::TestLogUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
  EXPECT_NE(ui.m_log_entries.size(), 0);
}

VarExistTest::VarExistTest()
    : mock_ui{}
{}

VarExistTest::~VarExistTest() = default;
