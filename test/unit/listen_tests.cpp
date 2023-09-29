/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Walter Van Herck (IO)
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
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/user_interface.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

//! Testing Listen instruction.
class ListenTest : public ::testing::Test
{
protected:
  ListenTest() = default;
  ~ListenTest() = default;
};

TEST_F(ListenTest, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("Listen");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  auto wait = GlobalInstructionRegistry().Create("Wait");
  ASSERT_TRUE(AppendChildInstruction(*instr, std::move(wait)));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("varNames", "var1,var2"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST_F(ListenTest, StopOnFailure)
{
  static const std::string procedure_body{
R"RAW(
  <Fallback>
    <ParallelSequence>
      <Listen varNames="monitor">
        <Inverter>
          <Equals leftVar="monitor" rightVar="update"/>
        </Inverter>
      </Listen>
      <Sequence>
        <Copy inputVar="update" outputVar="monitor"/>
      </Sequence>
      <Inverter>
        <Wait timeout="2.0"/>
      </Inverter>
    </ParallelSequence>
    <Equals leftVar="monitor" rightVar="update"/>
  </Fallback>
  <Workspace>
    <Local name="monitor"
           type='{"type":"uint64"}'
           value='0'/>
    <Local name="update"
           type='{"type":"uint64"}'
           value='1729'/>
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);

  sup::UnitTestHelper::EmptyUserInterface ui{};

  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_TRUE(
    sup::UnitTestHelper::TryAndExecuteNoReset(proc, ui, sup::sequencer::ExecutionStatus::SUCCESS));

  // check result variable
  auto ws = proc->GetWorkspace();
  ASSERT_NE(ws, nullptr);
  sup::dto::AnyValue result{sup::dto::UnsignedInteger64Type};
  EXPECT_TRUE(ws->GetValue("monitor", result));
  EXPECT_EQ(result.As<sup::dto::uint64>(), 1729u);
}

TEST_F(ListenTest, ForceSuccess)
{
  static const std::string procedure_body{
R"RAW(
  <Fallback>
    <ParallelSequence>
      <Listen varNames="result" forceSuccess="On">
          <Equals leftVar="result" rightVar="update"/>
      </Listen>
      <Inverter>
        <Sequence>
          <Wait timeout="0.1"/>
          <Copy inputVar="update" outputVar="result"/>
        </Sequence>
      </Inverter>
    </ParallelSequence>
    <Equals leftVar="result" rightVar="update"/>
  </Fallback>
  <Workspace>
    <Local name="update"
           type='{"type":"uint64"}'
           value='1729'/>
    <Local name="result"
           type='{"type":"uint64"}'
           value='0'/>
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);

  sup::UnitTestHelper::EmptyUserInterface ui{};

  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_TRUE(
    sup::UnitTestHelper::TryAndExecuteNoReset(proc, ui, sup::sequencer::ExecutionStatus::SUCCESS));

  // check result variable
  auto ws = proc->GetWorkspace();
  ASSERT_NE(ws, nullptr);
  sup::dto::AnyValue result{sup::dto::UnsignedInteger64Type};
  EXPECT_TRUE(ws->GetValue("result", result));
  EXPECT_EQ(result.As<sup::dto::uint64>(), 1729u);
}

TEST_F(ListenTest, PropagateSetup)
{
  static const std::string procedure_body{
R"RAW(
    <Sequence isRoot="True">
        <ForceSuccess>
            <Include name="Test Setup of Listen child" path="TestSetupListen"/>
        </ForceSuccess>
        <Equals leftVar="result" rightVar="one"/>
    </Sequence>
    <ParallelSequence name="TestSetupListen">
        <Listen varNames="monitor">
            <Fallback>
                <Equals leftVar="monitor" rightVar="zero" />
                <Include name="Set variable and fail" path="SetVarAndFail"/>
            </Fallback>
        </Listen>
        <Sequence>
            <Copy inputVar="one" outputVar="monitor"/>
        </Sequence>
        <Inverter>
            <Wait timeout="2.0" />
        </Inverter>
    </ParallelSequence>
    <Sequence name="SetVarAndFail">
        <Copy inputVar="one" outputVar="result"/>
        <Inverter>
            <Wait/>
        </Inverter>
    </Sequence>
    <Workspace>
        <Local name="monitor" type='{"type":"uint64"}' value='0' />
        <Local name="zero" type='{"type":"uint64"}' value='0' />
        <Local name="one" type='{"type":"uint64"}' value='1' />
        <Local name="result" type='{"type":"uint64"}' value='0' />
    </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);

  sup::UnitTestHelper::EmptyUserInterface ui{};

  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_TRUE(
    sup::UnitTestHelper::TryAndExecuteNoReset(proc, ui, sup::sequencer::ExecutionStatus::SUCCESS));

  // check result variable
  auto ws = proc->GetWorkspace();
  ASSERT_NE(ws, nullptr);
  sup::dto::AnyValue result{sup::dto::UnsignedInteger64Type};
  EXPECT_TRUE(ws->GetValue("result", result));
  EXPECT_EQ(result.As<sup::dto::uint64>(), 1u);
}

TEST_F(ListenTest, RepeatedListen)
{
  static const std::string procedure_body{
R"RAW(
    <Sequence>
        <Repeat maxCount="3">
            <Sequence>
                <Copy name="copy_zero" inputVar="zero" outputVar="live"/>
                <ParallelSequence name="outer">
                    <ParallelSequence successThreshold="1" failureThreshold="1">
                        <Inverter>
                            <Wait timeout="1.0"/>
                        </Inverter>
                        <Inverter>
                            <Listen varNames="live">
                                <Inverter>
                                    <Equals leftVar="live" rightVar="one"/>
                                </Inverter>
                            </Listen>
                        </Inverter>
                    </ParallelSequence>
                    <Sequence name="Set condition to true">
                        <Wait timeout="0.1"/>
                        <Copy name="copy_one" inputVar="one" outputVar="live"/>
                    </Sequence>
                </ParallelSequence>
                <Message text="Single success"/>
            </Sequence>
        </Repeat>
        <Message text="WaitForCondition successful!"/>
    </Sequence>
    <Workspace>
        <Local name="live" type='{"type":"uint64"}' value='0' />
        <Local name="zero" type='{"type":"uint64"}' value='0' />
        <Local name="one" type='{"type":"uint64"}' value='1' />
    </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);

  sup::UnitTestHelper::EmptyUserInterface ui{};

  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}
