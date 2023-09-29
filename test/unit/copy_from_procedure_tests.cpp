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

const std::string kTestExternalProcedureFilename = "test_copy_from_procedure.xml";

const std::string kTestExternalProcedureBody{R"(
  <Sequence name="InternalCopy">
      <Copy inputVar="a" outputVar="b"/>
  </Sequence>
  <Workspace>
      <Local name="a" type='{"type":"uint16"}' value='0' />
      <Local name="b" type='{"type":"uint16"}' value='0' />
  </Workspace>
)"};

class CopyFromProcedureTest : public ::testing::Test
{
protected:
  CopyFromProcedureTest();
  virtual ~CopyFromProcedureTest();
private:
  sup::UnitTestHelper::TemporaryTestFile m_test_file_1;
};

TEST_F(CopyFromProcedureTest, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("CopyFromProcedure");
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
  EXPECT_TRUE(instr->SetAttribute("file", "test_copy_from_procedure.xml"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST_F(CopyFromProcedureTest, MissingInputVar)
{
  const std::string body{R"(
    <CopyFromProcedure file="test_copy_from_procedure.xml" inputVar="does_not_exist" outputVar="c"/>
    <Workspace>
        <Local name="c" type='{"type":"uint16"}' value='2' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(CopyFromProcedureTest, MissingOutputVar)
{
  const std::string body{R"(
    <CopyFromProcedure file="test_copy_from_procedure.xml" inputVar="a" outputVar="does_not_exist"/>
    <Workspace>
        <Local name="c" type='{"type":"uint16"}' value='2' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(CopyFromProcedureTest, Success)
{
  const std::string body{R"(
    <CopyFromProcedure file="test_copy_from_procedure.xml" inputVar="b" outputVar="c"/>
    <Workspace>
        <Local name="c" type='{"type":"uint16"}' value='2' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(CopyFromProcedureTest, SuccessBothWays)
{
  const std::string body{R"(
    <Sequence>
        <Inverter>
            <Equals leftVar="result" rightVar="c"/>
        </Inverter>
        <CopyToProcedure file="test_copy_from_procedure.xml" inputVar="c" outputVar="a"/>
        <IncludeProcedure file="test_copy_from_procedure.xml" path="InternalCopy"/>
        <CopyFromProcedure file="test_copy_from_procedure.xml" inputVar="b" outputVar="result"/>
        <Equals leftVar="result" rightVar="c"/>
    </Sequence>
    <Workspace>
        <Local name="c" type='{"type":"uint16"}' value='2' />
        <Local name="result" type='{"type":"uint16"}' value='0' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

CopyFromProcedureTest::CopyFromProcedureTest()
  : m_test_file_1{kTestExternalProcedureFilename,
                  sup::UnitTestHelper::CreateProcedureString(kTestExternalProcedureBody)}
{}

CopyFromProcedureTest::~CopyFromProcedureTest() = default;