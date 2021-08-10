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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "LogUI.h"
#include "UnitTestHelper.h"

#include <SequenceParser.h>
#include <common/BasicTypes.h>
#include <common/SysTools.h>
#include <gtest/gtest.h>

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

  sup::sequencer::LogUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

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

  sup::sequencer::LogUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

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

  sup::sequencer::LogUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

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

  sup::sequencer::LogUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}

TEST(Include, Procedure_extern)
{
  // preparing test file for inclusion
  const std::string body{R"(
    <ParallelSequence name="Parallel Wait" successThreshold="2">
        <Wait name="One" timeout="0.1" />
        <Wait name="Two" timeout="0.2" />
        <Wait name="Three" timeout="3.0" />
    </ParallelSequence>
    <Workspace>
    </Workspace>
)"};

  const std::string file_name = "/tmp/parallel_sequence.xml";
  ::sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, ::sup::UnitTestHelper::CreateProcedureString(body));

  const std::string procedure_body{R"(
    <Repeat isRoot="true" maxCount="10">
        <Include name="Waits" path="Parallel Wait" file="/tmp/parallel_sequence.xml"/>
    </Repeat>
    <Workspace>
    </Workspace>
)"};

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      ::sup::UnitTestHelper::CreateProcedureString(procedure_body));
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}

TEST(Include, Procedure_nested)
{
  // preparing files for recursive inclusion
  ccs::HelperTools::CreatePath("/tmp/instruction_definitions");
  ccs::HelperTools::CreatePath("/tmp/instruction_definitions/waits");

  const std::string single_waits_body{R"(
    <Wait name="OneTenthSecond" timeout="0.1" />
    <Wait name="TwoTenthSeconds" timeout="0.2" />
    <Wait name="ThreeTenthSeconds" timeout="0.3" />
    <Wait name="ParametrizedWait" timeout="$timeout" />
)"};

  const std::string single_wait_file_name = "/tmp/instruction_definitions/waits/single_waits.xml";
  ::sup::UnitTestHelper::TemporaryTestFile single_waits_test_file(
      single_wait_file_name, ::sup::UnitTestHelper::CreateProcedureString(single_waits_body));

  const std::string compound_waits_body{R"(
    <Sequence name="SerialWait">
        <Include name="One" path="OneTenthSecond" file="waits/single_waits.xml" />
        <Include name="Two" path="ParametrizedWait" file="waits/single_waits.xml" timeout="$par1" />
    </Sequence>
    <ParallelSequence name="ParallelWait">
        <Include name="One" path="TwoTenthSeconds" file="waits/single_waits.xml" />
        <Include name="Two" path="ParametrizedWait" file="waits/single_waits.xml" timeout="$par1" />
    </ParallelSequence>
)"};

  const std::string compound_waits_file_name = "/tmp/instruction_definitions/compound_waits.xml";
  ::sup::UnitTestHelper::TemporaryTestFile compound_waits_test_file(
      compound_waits_file_name, ::sup::UnitTestHelper::CreateProcedureString(compound_waits_body));

  const std::string main_body{R"(
    <Sequence isRoot="True">
        <Include name="First wait" path="SerialWait"
                 file="instruction_definitions/compound_waits.xml" par1="0.4" />
        <Include name="Second Wait" path="ParallelWait"
                 file="instruction_definitions/compound_waits.xml" par1="0.4" />
    </Sequence>
)"};

  const std::string file_name = "/tmp/recursive_include.xml";
  ::sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, ::sup::UnitTestHelper::CreateProcedureString(main_body));

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureFile(file_name);

  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}
