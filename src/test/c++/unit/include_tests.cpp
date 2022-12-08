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
 * Copyright (c) : 2010-2022 ITER Organization,
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

TEST(Include, SetupSuccessful)
{
  const std::string body{R"(
    <Sequence name="DontWait">
        <Wait/>
    </Sequence>
    <Include isRoot="true" name="Counts" path="DontWait"/>
    <Workspace>
    </Workspace>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_NO_THROW(proc->Setup());
}

TEST(Include, VariableFromInclude)
{
  const std::string body{R"(
    <Sequence name="DontWait">
        <Wait timeout="$to" />
    </Sequence>
    <Include isRoot="true" name="Counts" path="DontWait" to="0.2"/>
    <Workspace>
    </Workspace>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_NO_THROW(proc->Setup());
}

TEST(Include, MissingPath)
{
  const std::string body{R"(
    <Sequence name="DontWait">
        <Wait/>
    </Sequence>
    <Include isRoot="true" name="Counts"/>
    <Workspace>
    </Workspace>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_THROW(proc->Setup(), InstructionSetupException);
}

TEST(Include, WrongPath)
{
  const std::string body{R"(
    <Sequence name="DontWait">
        <Wait/>
    </Sequence>
    <Include isRoot="true" name="Counts" path="DoesNotExist"/>
    <Workspace>
    </Workspace>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_THROW(proc->Setup(), InstructionSetupException);
}

TEST(Include, MissingVariableFromInclude)
{
  const std::string body{R"(
    <Sequence name="DontWait">
        <Wait timeout="$to" />
    </Sequence>
    <Include isRoot="true" name="Counts" path="DontWait"/>
    <Workspace>
    </Workspace>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_THROW(proc->Setup(), InstructionSetupException);
}

TEST(Include, Procedure_local)
{
  const std::string body{R"(
    <Sequence name="CountTwice">
        <Counter/>
        <Counter/>
    </Sequence>
    <Repeat isRoot="true" maxCount="10">
        <Include name="Counts" path="CountTwice"/>
    </Repeat>
    <Workspace>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 20);
}

TEST(Include, Procedure_param)
{
  const std::string body{R"(
    <Sequence name="CountParam">
        <Counter incr="$incr"/>
    </Sequence>
    <Repeat isRoot="true" maxCount="10">
        <Include name="Counts" path="CountParam" incr="2"/>
    </Repeat>
    <Workspace>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 20);
}

TEST(Include, Procedure_decorator)
{
  const std::string body{R"(
    <ForceSuccess name="CountParamWithDecorator">
        <Counter incr="$incr"/>
    </ForceSuccess>
    <Repeat isRoot="true" maxCount="10">
        <Include name="Counts" path="CountParamWithDecorator"
incr="2"/>
    </Repeat>
    <Workspace>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 20);
}

TEST(Include, Procedure_undefined)
{
  const std::string body{R"(
    <Sequence isRoot="true">
        <Include name="undefined" path="undefined"/>
        <Include name="undefined" file="undefined"/>
        <Include name="undefined" path="undefined" file="undefined"/>
    </Sequence>
    <Workspace>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_THROW(sup::UnitTestHelper::TryAndExecute(proc, &ui), InstructionSetupException);
}

TEST(Include, Procedure_extern)
{
  // preparing test file for inclusion
  const std::string body{R"(
    <Sequence name="Double Wait">
        <Wait name="One"/>
        <Wait name="Two"/>
    </Sequence>
    <Workspace>
    </Workspace>
)"};

  const std::string file_name = "sequence.xml";
  sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, sup::UnitTestHelper::CreateProcedureString(body));

  const std::string procedure_body{R"(
    <Repeat isRoot="true" maxCount="5">
        <Include name="Waits" path="Double Wait" file="sequence.xml"/>
    </Repeat>
    <Workspace>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(procedure_body));
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}

TEST(Include, Procedure_nested)
{
  // preparing files for recursive inclusion
  ASSERT_TRUE(utils::CreateDir("instruction_definitions/waits"));

  const std::string single_waits_body{R"(
    <Wait name="OneTenthSecond"/>
    <Wait name="TwoTenthSeconds"/>
    <Wait name="ThreeTenthSeconds"/>
    <Wait name="ParametrizedWait" timeout="$timeout" />
)"};

  const std::string single_wait_file_name = "instruction_definitions/waits/single_waits.xml";
  sup::UnitTestHelper::TemporaryTestFile single_waits_test_file(
      single_wait_file_name, sup::UnitTestHelper::CreateProcedureString(single_waits_body));

  const std::string compound_waits_body{R"(
    <Sequence name="FirstSerialWait">
        <Include name="One" path="OneTenthSecond" file="waits/single_waits.xml" />
        <Include name="Two" path="ParametrizedWait" file="waits/single_waits.xml" timeout="$par1" />
    </Sequence>
    <Sequence name="SecondSerialWait">
        <Include name="One" path="TwoTenthSeconds" file="waits/single_waits.xml" />
        <Include name="Two" path="ParametrizedWait" file="waits/single_waits.xml" timeout="$par1" />
    </Sequence>
)"};

  const std::string compound_waits_file_name = "instruction_definitions/compound_waits.xml";
  sup::UnitTestHelper::TemporaryTestFile compound_waits_test_file(
      compound_waits_file_name, sup::UnitTestHelper::CreateProcedureString(compound_waits_body));

  const std::string main_body{R"(
    <Sequence isRoot="True">
        <Include name="First wait" path="FirstSerialWait"
                 file="instruction_definitions/compound_waits.xml" par1="0.01" />
        <Include name="Second Wait" path="SecondSerialWait"
                 file="instruction_definitions/compound_waits.xml" par1="0.01" />
    </Sequence>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(main_body);

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(proc_str);

  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}
