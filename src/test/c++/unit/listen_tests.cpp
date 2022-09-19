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

#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/user_interface.h>

#include <common/BasicTypes.h>

#include <gtest/gtest.h>

//! Testing Listen instruction.
class ListenTest : public ::testing::Test
{
protected:
  ListenTest() = default;
  ~ListenTest() = default;
};

TEST_F(ListenTest, StopOnFailure)
{
  static const std::string procedure_body{
R"RAW(
  <Fallback>
    <ParallelSequence>
      <Listen varNames="monitor">
        <Inverter>
          <Equals lhs="monitor" rhs="update"/>
        </Inverter>
      </Listen>
      <Sequence>
        <Wait timeout="0.2"/>
        <Copy input="update" output="monitor"/>
      </Sequence>
      <Inverter>
        <Wait timeout="2.0"/>
      </Inverter>
    </ParallelSequence>
    <Equals lhs="monitor" rhs="update"/>
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

  const auto procedure_string = ::sup::UnitTestHelper::CreateProcedureString(procedure_body);

  ::sup::UnitTestHelper::EmptyUserInterface ui{};

  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_TRUE(
    sup::UnitTestHelper::TryAndExecuteNoReset(proc, &ui, sup::sequencer::ExecutionStatus::SUCCESS));

  // check result variable
  auto ws = proc->GetWorkspace();
  ASSERT_NE(ws, nullptr);
  sup::dto::AnyValue result{sup::dto::UnsignedInteger64Type};
  EXPECT_TRUE(ws->GetValue("monitor", result));
  EXPECT_EQ(result, (sup::dto::uint64)1729);
  proc->Reset();
}

TEST_F(ListenTest, ForceSuccess)
{
  static const std::string procedure_body{
R"RAW(
  <Fallback>
    <ParallelSequence>
      <Listen varNames="monitor" forceSuccess="On">
        <Sequence>
          <Copy input="monitor" output="result"/>
          <Equals lhs="result" rhs="update"/>
        </Sequence>
      </Listen>
      <Sequence>
        <Wait timeout="0.2"/>
        <Copy input="update" output="monitor"/>
      </Sequence>
      <Inverter>
        <Wait timeout="0.5"/>
      </Inverter>
    </ParallelSequence>
    <Equals lhs="result" rhs="update"/>
  </Fallback>
  <Workspace>
    <Local name="monitor"
           type='{"type":"uint64"}'
           value='0'/>
    <Local name="update"
           type='{"type":"uint64"}'
           value='1729'/>
    <Local name="result"
           type='{"type":"uint64"}'/>
  </Workspace>
)RAW"};

  const auto procedure_string = ::sup::UnitTestHelper::CreateProcedureString(procedure_body);

  ::sup::UnitTestHelper::EmptyUserInterface ui{};

  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_TRUE(
    sup::UnitTestHelper::TryAndExecuteNoReset(proc, &ui, sup::sequencer::ExecutionStatus::SUCCESS));

  // check result variable
  auto ws = proc->GetWorkspace();
  ASSERT_NE(ws, nullptr);
  sup::dto::AnyValue result{sup::dto::UnsignedInteger64Type};
  EXPECT_TRUE(ws->GetValue("result", result));
  EXPECT_EQ(result, (sup::dto::uint64)1729);
  proc->Reset();
}

TEST_F(ListenTest, PropagateSetup)
{
  static const std::string procedure_body{
R"RAW(
    <Sequence isRoot="True">
        <ForceSuccess>
            <Include name="Test Setup of Listen child" path="TestSetupListen"/>
        </ForceSuccess>
        <Equals lhs="result" rhs="one"/>
    </Sequence>
    <ParallelSequence name="TestSetupListen">
        <Listen varNames="monitor">
            <Fallback>
                <Equals lhs="monitor" rhs="zero" />
                <Include name="Set variable and fail" path="SetVarAndFail"/>
            </Fallback>
        </Listen>
        <Sequence>
            <Wait timeout="0.2" />
            <Copy input="one" output="monitor"/>
        </Sequence>
        <Inverter>
            <Wait timeout="2.0" />
        </Inverter>
    </ParallelSequence>
    <Sequence name="SetVarAndFail">
        <Copy input="one" output="result"/>
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

  const auto procedure_string = ::sup::UnitTestHelper::CreateProcedureString(procedure_body);

  ::sup::UnitTestHelper::EmptyUserInterface ui{};

  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_TRUE(
    sup::UnitTestHelper::TryAndExecuteNoReset(proc, &ui, sup::sequencer::ExecutionStatus::SUCCESS));

  // check result variable
  auto ws = proc->GetWorkspace();
  ASSERT_NE(ws, nullptr);
  sup::dto::AnyValue result{sup::dto::UnsignedInteger64Type};
  EXPECT_TRUE(ws->GetValue("result", result));
  EXPECT_EQ(result, (sup::dto::uint64)1);
  proc->Reset();
}
