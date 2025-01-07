/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Bruno Santos
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

#include "unit_test_helper.h"

#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

// When the outer listen is triggered, the inner listen can also be resolved
// immediately. Block resolves after both resolution conditions are met as
// expected. This also implies that the nested listen trigger is actuated in
// delay.
TEST(NestedListenTest, InnerConditionMetFirst)
{
  static const std::string procedure_body{
R"RAW(
  <ParallelSequence>
    <Inverter>
      <Listen varNames="outer_exit_cond">
        <Inverter>
          <Sequence>
            <Equals leftVar="outer_exit_cond" rightVar="one"/>
            <Inverter>
              <Listen varNames="inner_exit_cond">
                  <Inverter>
                    <Equals leftVar="inner_exit_cond" rightVar="one"/>
                  </Inverter>
              </Listen>
            </Inverter>
          </Sequence>
        </Inverter>
      </Listen>
    </Inverter>
    <Sequence>
      <Copy inputVar="one" outputVar="inner_exit_cond"/>
      <Wait timeout="0.1"/>
      <Copy inputVar="one" outputVar="outer_exit_cond"/>
    </Sequence>
  </ParallelSequence>
  <Workspace>
    <Local name="outer_exit_cond"
           type='{"type":"uint64"}'
           value='0'/>
    <Local name="inner_exit_cond"
           type='{"type":"uint64"}'
           value='0'/>
    <Local name="one"
           type='{"type":"uint64"}'
           value='1'/>
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);

  sup::UnitTestHelper::EmptyUserInterface ui{};

  auto proc = ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_TRUE(
    sup::UnitTestHelper::TryAndExecuteNoReset(proc, ui, ExecutionStatus::SUCCESS));

  // check result variable
  auto& ws = proc->GetWorkspace();
  sup::dto::AnyValue result{sup::dto::UnsignedInteger64Type};
  EXPECT_TRUE(ws.GetValue("outer_exit_cond", result));
  EXPECT_EQ(result.As<sup::dto::uint64>(), 1u);
  EXPECT_TRUE(ws.GetValue("inner_exit_cond", result));
  EXPECT_EQ(result.As<sup::dto::uint64>(), 1u);
}

// When the outer listen is triggered, the inner listen cannot yet be resolved.
// Block resolves after both resolution conditions are met as expected.
TEST(NestedListenTest, InnerConditionMetLast)
{
  static const std::string procedure_body{
R"RAW(
  <ParallelSequence>
    <Inverter>
      <Listen varNames="outer_exit_cond">
        <Inverter>
          <Sequence>
            <Equals leftVar="outer_exit_cond" rightVar="one"/>
            <Copy inputVar="one" outputVar="inner_exit_cond"/>
            <Inverter>
              <Listen varNames="inner_exit_cond">
                  <Inverter>
                    <Equals leftVar="inner_exit_cond" rightVar="one"/>
                  </Inverter>
              </Listen>
            </Inverter>
          </Sequence>
        </Inverter>
      </Listen>
    </Inverter>
    <Sequence>
      <Copy inputVar="one" outputVar="outer_exit_cond"/>
    </Sequence>
  </ParallelSequence>
  <Workspace>
    <Local name="outer_exit_cond"
           type='{"type":"uint64"}'
           value='0'/>
    <Local name="inner_exit_cond"
           type='{"type":"uint64"}'
           value='0'/>
    <Local name="one"
           type='{"type":"uint64"}'
           value='1'/>
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);

  sup::UnitTestHelper::EmptyUserInterface ui{};

  auto proc = ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_TRUE(
    sup::UnitTestHelper::TryAndExecuteNoReset(proc, ui, ExecutionStatus::SUCCESS));

  // check result variable
  auto& ws = proc->GetWorkspace();
  sup::dto::AnyValue result{sup::dto::UnsignedInteger64Type};
  EXPECT_TRUE(ws.GetValue("outer_exit_cond", result));
  EXPECT_EQ(result.As<sup::dto::uint64>(), 1u);
  EXPECT_TRUE(ws.GetValue("inner_exit_cond", result));
  EXPECT_EQ(result.As<sup::dto::uint64>(), 1u);
}

// Test nested listen conditions for serial resolution. The outer listen can
// only return if the inner one cannot and vice versa. However they are executed
// serially, so we can set the condition sequentially so that the outer listen
// checks its return condition before it waits on the inner listen, which will
// in turn await its own return condition. Meeting both conditions in order
// results in a block return despite the fact that the outer condition is no
// longer met by the time the inner one is met.
TEST(NestedListenTest, MutuallyExclusiveConditionsMetSeriallyInOrder)
{
  static const std::string procedure_body{
R"RAW(
  <ParallelSequence>
    <Inverter>
      <Listen varNames="exit_cond">
        <Inverter>
          <Sequence>
            <Equals leftVar="exit_cond" rightVar="one"/>
            <Copy inputVar="zero" outputVar="exit_cond"/>
            <Inverter>
              <Listen varNames="exit_cond">
                <Sequence>
                  <Inverter>
                    <Equals leftVar="exit_cond" rightVar="zero"/>
                  </Inverter>
                </Sequence>
              </Listen>
            </Inverter>
          </Sequence>
        </Inverter>
      </Listen>
    </Inverter>
    <Sequence>
      <Copy inputVar="one" outputVar="exit_cond"/>
    </Sequence>
  </ParallelSequence>
  <Workspace>
    <Local name="exit_cond"
           type='{"type":"uint64"}'
           value='42'/>
    <Local name="one"
           type='{"type":"uint64"}'
           value='1'/>
    <Local name="zero"
           type='{"type":"uint64"}'
           value='0'/>
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);

  sup::UnitTestHelper::EmptyUserInterface ui{};

  auto proc = ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_TRUE(
    sup::UnitTestHelper::TryAndExecuteNoReset(proc, ui, ExecutionStatus::SUCCESS));

  // check result variable
  auto& ws = proc->GetWorkspace();
  sup::dto::AnyValue result{sup::dto::UnsignedInteger64Type};
  EXPECT_TRUE(ws.GetValue("exit_cond", result));
  EXPECT_EQ(result.As<sup::dto::uint64>(), 0u);
}

// Like `MutuallyExclusiveConditionsMetSeriallyInOrder` except the inner
// condition is met first, but not checked then as it's conditioned on the outer
// condition. This results in the block never returning, which is detected by
// "listen_stuck_monitor".
TEST(NestedListenTest, MutuallyExclusiveConditionsMetSeriallyOutOfOrder)
{
  static const std::string procedure_body{
R"RAW(
  <ParallelSequence>
    <ParallelSequence successThreshold="1">
      <Inverter>
        <Listen varNames="exit_cond">
          <Inverter>
            <Sequence>
              <Equals leftVar="exit_cond" rightVar="zero"/>
              <Inverter>
                <Listen varNames="exit_cond">
                  <Inverter>
                    <Equals leftVar="exit_cond" rightVar="one"/>
                  </Inverter>
                </Listen>
              </Inverter>
            </Sequence>
          </Inverter>
        </Listen>
      </Inverter>
      <Sequence>
        <Wait timeout="0.2"/>
        <Copy inputVar="one" outputVar="listen_stuck_monitor"/>
      </Sequence>
    </ParallelSequence>
    <Sequence>
      <Copy inputVar="one" outputVar="exit_cond"/>
      <Wait timeout="0.1"/>
      <Copy inputVar="zero" outputVar="exit_cond"/>
    </Sequence>
  </ParallelSequence>
  <Workspace>
    <Local name="listen_stuck_monitor"
           type='{"type":"uint64"}'
           value='0'/>
    <Local name="exit_cond"
           type='{"type":"uint64"}'
           value='42'/>
    <Local name="one"
           type='{"type":"uint64"}'
           value='1'/>
    <Local name="zero"
           type='{"type":"uint64"}'
           value='0'/>
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);

  sup::UnitTestHelper::EmptyUserInterface ui{};

  auto proc = ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_TRUE(
    sup::UnitTestHelper::TryAndExecuteNoReset(proc, ui, ExecutionStatus::SUCCESS));

  // check result variable
  auto& ws = proc->GetWorkspace();
  sup::dto::AnyValue result{sup::dto::UnsignedInteger64Type};
  EXPECT_TRUE(ws.GetValue("exit_cond", result));
  EXPECT_EQ(result.As<sup::dto::uint64>(), 0u);
  EXPECT_TRUE(ws.GetValue("listen_stuck_monitor", result));
  EXPECT_EQ(result.As<sup::dto::uint64>(), 1u);
}
