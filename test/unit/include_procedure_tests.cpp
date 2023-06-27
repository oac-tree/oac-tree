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

#include <gtest/gtest.h>

using namespace sup::sequencer;

const std::string kTestProcedureFileName = "test_procedure.xml";

const std::string kTestProcedureBody{R"(
  <Sequence name="CopyAndCheck">
      <Copy input="a" output="b"/>
      <Equals lhs="a" rhs="b"/>
      <Wait timeout="1.0"/>
  </Sequence>
  <Workspace>
      <Local name="a" type='{"type":"uint16"}' value='1' />
      <Local name="b" type='{"type":"uint16"}' value='0' />
  </Workspace>
)"};

class IncludeProcedureTest : public ::testing::Test
{
protected:
  IncludeProcedureTest();
  virtual ~IncludeProcedureTest();
private:
  sup::UnitTestHelper::TemporaryTestFile m_test_file;
};

TEST_F(IncludeProcedureTest, RootPath)
{
  const std::string body{R"(
    <IncludeProcedure file="test_procedure.xml"/>
    <Workspace>
    </Workspace>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  sup::UnitTestHelper::EmptyUserInterface ui;
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

IncludeProcedureTest::IncludeProcedureTest()
  : m_test_file{kTestProcedureFileName,
                sup::UnitTestHelper::CreateProcedureString(kTestProcedureBody)}
{}

IncludeProcedureTest::~IncludeProcedureTest() = default;