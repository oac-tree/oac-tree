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

const std::string kTestExternalProcedureFilename = "test_copy_to_procedure.xml";

const std::string kTestExternalProcedureBody{R"(
  <Wait name="DummyWait"/>
  <Workspace>
      <Local name="a" type='{"type":"uint16"}' value='0' />
      <Local name="b" type='{"type":"uint16"}' value='1' />
  </Workspace>
)"};

class CopyToProcedureTest : public ::testing::Test
{
protected:
  CopyToProcedureTest();
  virtual ~CopyToProcedureTest();
private:
  sup::UnitTestHelper::TemporaryTestFile m_test_file_1;
};

TEST_F(CopyToProcedureTest, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("CopyToProcedure");
  ASSERT_NE(instr.get(), nullptr);

  // Missing file attribute
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  // Correct file attribute, missing input/output
  EXPECT_TRUE(instr->AddAttribute("file", "some_file.xml"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  // Correct input attribute, missing output
  EXPECT_TRUE(instr->AddAttribute("inputVar", "in_var"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  // Correct output attribute, but file doesn't exist
  EXPECT_TRUE(instr->AddAttribute("outputVar", "out_var"));
  EXPECT_THROW(instr->Setup(proc), sup::xml::ParseException);

  // Existing filename works
  EXPECT_TRUE(instr->SetAttribute("file", "test_copy_to_procedure.xml"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST_F(CopyToProcedureTest, MissingInputVar)
{
  const std::string body{R"(
    <CopyToProcedure file="test_copy_to_procedure.xml" inputVar="does_not_exist" outputVar="a"/>
    <Workspace>
        <Local name="c" type='{"type":"uint16"}' value='2' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(CopyToProcedureTest, MissingOutputVar)
{
  const std::string body{R"(
    <CopyToProcedure file="test_copy_to_procedure.xml" inputVar="c" outputVar="does_not_exist"/>
    <Workspace>
        <Local name="c" type='{"type":"uint16"}' value='2' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(CopyToProcedureTest, Success)
{
  const std::string body{R"(
    <CopyToProcedure file="test_copy_to_procedure.xml" inputVar="c" outputVar="a"/>
    <Workspace>
        <Local name="c" type='{"type":"uint16"}' value='2' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

CopyToProcedureTest::CopyToProcedureTest()
  : m_test_file_1{kTestExternalProcedureFilename,
                  sup::UnitTestHelper::CreateProcedureString(kTestExternalProcedureBody)}
{}

CopyToProcedureTest::~CopyToProcedureTest() = default;
