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


#include <sup/sequencer/runner.h>

#include "mock_user_interface.h"
#include "unit_test_helper.h"

#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

#include <chrono>
#include <functional>
#include <future>
#include <thread>

using namespace sup::sequencer;
using namespace sup::UnitTestHelper;

class RunnerTest : public ::testing::Test
{
protected:
  RunnerTest();
  virtual ~RunnerTest();

  MockUserInterface mock_ui;
  EmptyUserInterface empty_ui;
  std::unique_ptr<Procedure> async_proc;
  std::unique_ptr<Procedure> sync_proc;
  std::unique_ptr<Procedure> copy_proc;
  std::unique_ptr<Procedure> async_wait_proc;
};

const std::string AsyncProcedureString =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Asynchronous procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd"
           tickTimeout="0.01">
    <Sequence name="Main Sequence">
        <Wait name="Immediate Success"/>
        <ParallelSequence name="Parallel Wait" successThreshold="1">
            <Wait name="One"/>
            <Wait name="Two" timeout="2.0"/>
        </ParallelSequence>
    </Sequence>
</Procedure>
)RAW";

const std::string SyncProcedureString =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Synchronous procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Sequence name="Main Sequence">
        <Wait name="Immediate Success"/>
        <Inverter name="Invert success">
            <Wait name="Fail immediately"/>
        </Inverter>
        <Wait name="Will never be called" timeout="2.0"/>
    </Sequence>
</Procedure>
)RAW";

const std::string CopyVariableProcedureString =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Sequence>
        <Copy name="Copy workspace variables" inputVar="var1" outputVar="var2" />
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint64"}'
               value='1729' />
        <Local name="var2"
               type='{"type":"uint64"}' />
    </Workspace>
</Procedure>
)RAW";

const std::string AsyncWaitProcedureString =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Synchronous procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd"
           tickTimeout="0.01">
    <ParallelSequence name="Main Sequence">
        <Wait name="Immediate Success" timeout="5.0"/>
        <Wait name="Immediate Success" timeout="10.0"/>
        <Wait name="Immediate Success" timeout="10.0"/>
    </ParallelSequence>
</Procedure>
)RAW";

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::InSequence;

TEST_F(RunnerTest, NoProcedure)
{
  // Test constructed
  Runner runner(mock_ui);
  EXPECT_TRUE(runner.IsFinished());  // empty procedure is finished by default
  EXPECT_FALSE(runner.IsRunning());
  EXPECT_NO_THROW(runner.ExecuteProcedure());
  EXPECT_TRUE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());
  EXPECT_NO_THROW(runner.ExecuteSingle());
  EXPECT_TRUE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());

  // Trying to set breakpoints at non-existing instructions
  EXPECT_THROW(runner.SetBreakpoint(nullptr), InvalidOperationException);
  auto instr = GlobalInstructionRegistry().Create("Wait");
  ASSERT_NE(instr.get(), nullptr);
  EXPECT_THROW(runner.SetBreakpoint(instr.get()), InvalidOperationException);
}

TEST_F(RunnerTest, ExecuteSingle)
{
  // Set Expectations on mock UserInterface calls
  EXPECT_CALL(mock_ui, UpdateInstructionStatus(_)).Times(AtLeast(12));

  // Test preconditions
  Runner runner(mock_ui);
  EXPECT_TRUE(runner.IsFinished());  // empty procedure is finished by default
  EXPECT_FALSE(runner.IsRunning());

  // Set procedure and test conditions again
  EXPECT_NO_THROW(async_proc->Setup());
  EXPECT_NO_THROW(runner.SetProcedure(async_proc.get()));
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());

  // Execute single synchronous instruction
  EXPECT_NO_THROW(runner.ExecuteSingle());
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());

  // Execute parallel sequence
  EXPECT_NO_THROW(runner.ExecuteSingle());
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_TRUE(runner.IsRunning());

  // Wait for non-running state and test if finished
  while (runner.IsRunning())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_NO_THROW(runner.ExecuteSingle());
  }
  EXPECT_TRUE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());
}

TEST_F(RunnerTest, ExecuteProcedure)
{
  // Set Expectations on mock UserInterface calls
  EXPECT_CALL(mock_ui, UpdateInstructionStatus(_)).Times(AtLeast(12));

  // Test preconditions
  Runner runner(mock_ui);
  EXPECT_TRUE(runner.IsFinished());  // empty procedure is finished by default
  EXPECT_FALSE(runner.IsRunning());

  // Set procedure and test conditions again
  EXPECT_NO_THROW(async_proc->Setup());
  EXPECT_NO_THROW(runner.SetProcedure(async_proc.get()));
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());

  // Execute whole procedure
  EXPECT_NO_THROW(runner.ExecuteProcedure());
  EXPECT_TRUE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());
}

TEST_F(RunnerTest, UICalls)
{
  // Set Expectations on mock UserInterface calls
  {
    InSequence seq;
    EXPECT_CALL(mock_ui,
                UpdateInstructionStatus(HasExecutionStatus(ExecutionStatus::NOT_FINISHED)));
    EXPECT_CALL(mock_ui,
                UpdateInstructionStatus(HasExecutionStatus(ExecutionStatus::NOT_FINISHED)));
    EXPECT_CALL(mock_ui, UpdateInstructionStatus(HasExecutionStatus(ExecutionStatus::SUCCESS)));
    EXPECT_CALL(mock_ui,
                UpdateInstructionStatus(HasExecutionStatus(ExecutionStatus::NOT_FINISHED)));
    EXPECT_CALL(mock_ui,
                UpdateInstructionStatus(HasExecutionStatus(ExecutionStatus::NOT_FINISHED)));
    EXPECT_CALL(mock_ui, UpdateInstructionStatus(HasExecutionStatus(ExecutionStatus::SUCCESS)));
    EXPECT_CALL(mock_ui, UpdateInstructionStatus(HasExecutionStatus(ExecutionStatus::FAILURE)));
    EXPECT_CALL(mock_ui, UpdateInstructionStatus(HasExecutionStatus(ExecutionStatus::FAILURE)));
  }

  // Test preconditions
  Runner runner(mock_ui);
  EXPECT_TRUE(runner.IsFinished());  // empty procedure is finished by default
  EXPECT_FALSE(runner.IsRunning());

  // Set procedure and test conditions again
  EXPECT_NO_THROW(sync_proc->Setup());
  EXPECT_NO_THROW(runner.SetProcedure(sync_proc.get()));
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());

  // Execute whole procedure
  EXPECT_NO_THROW(runner.ExecuteProcedure());
  EXPECT_TRUE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());
}

TEST_F(RunnerTest, UIVariableCalls)
{
  // Set Expectations on mock UserInterface calls
  sup::dto::AnyValue val(sup::dto::UnsignedInteger64Type);
  val = sup::dto::uint64(1729);
  EXPECT_CALL(mock_ui, VariableUpdated("var2", HasSameValue(val), true)).Times(Exactly(1));
  EXPECT_CALL(mock_ui, UpdateInstructionStatus(_)).Times(AtLeast(4));

  // Test preconditions
  Runner runner(mock_ui);
  EXPECT_TRUE(runner.IsFinished());  // empty procedure is finished by default
  EXPECT_FALSE(runner.IsRunning());

  // Set procedure and test conditions again
  EXPECT_NO_THROW(copy_proc->Setup());
  EXPECT_NO_THROW(runner.SetProcedure(copy_proc.get()));
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());

  // Execute whole procedure
  EXPECT_NO_THROW(runner.ExecuteProcedure());
  EXPECT_TRUE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());
}

TEST_F(RunnerTest, Halt)
{
  // Test preconditions
  Runner runner(empty_ui);
  EXPECT_TRUE(runner.IsFinished());  // empty procedure is finished by default
  EXPECT_FALSE(runner.IsRunning());

  // Set procedure and test conditions again
  EXPECT_NO_THROW(async_wait_proc->Setup());
  EXPECT_NO_THROW(runner.SetProcedure(async_wait_proc.get()));
  EXPECT_EQ(async_wait_proc->GetStatus(), ExecutionStatus::NOT_STARTED);
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());

  // Setup separate thread to halt the execution after a second
  std::promise<void> ready, go;
  auto go_future = go.get_future();
  auto halt_future = std::async(std::launch::async,
    [this, &runner, &ready, &go_future](){
      ready.set_value();
      go_future.get();
      while(async_wait_proc->GetStatus() != ExecutionStatus::RUNNING)
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      runner.Halt();
    });

  // Execute whole procedure
  ready.get_future().get();
  go.set_value();
  EXPECT_NO_THROW(runner.ExecuteProcedure());
  halt_future.get();
  runner.ExecuteSingle();  // retrieves statuses of asynchronous instructions
  int max_waits = 100;
  while (async_wait_proc->GetStatus() == ExecutionStatus::RUNNING && max_waits > 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    runner.ExecuteSingle();  // retrieves statuses of asynchronous instructions
    --max_waits;
  }
  EXPECT_EQ(async_wait_proc->GetStatus(), ExecutionStatus::FAILURE);
  EXPECT_TRUE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());
}

TEST_F(RunnerTest, Pause)
{
  // Test preconditions
  Runner runner(empty_ui);
  EXPECT_TRUE(runner.IsFinished());  // empty procedure is finished by default
  EXPECT_FALSE(runner.IsRunning());

  // Set procedure and test conditions again
  EXPECT_NO_THROW(sync_proc->Setup());
  EXPECT_NO_THROW(runner.SetProcedure(sync_proc.get()));
  EXPECT_EQ(sync_proc->GetStatus(), ExecutionStatus::NOT_STARTED);
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());

  // Set callback to pause runner and execute
  auto cb = [&runner](const Procedure&) { runner.Pause(); };
  runner.SetTickCallback(cb);
  EXPECT_NO_THROW(runner.ExecuteProcedure());
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());
  // Query next instructions list
  auto next_instructions = GetNextInstructions(*sync_proc);
  EXPECT_EQ(next_instructions.size(), 3);
  // Query leaves
  auto next_leaves = GetNextLeaves(*sync_proc);
  ASSERT_EQ(next_leaves.size(), 1);
  auto found_it = std::find(next_instructions.begin(), next_instructions.end(), next_leaves[0]);
  EXPECT_NE(found_it, next_instructions.end());

  // Start again
  EXPECT_NO_THROW(runner.ExecuteProcedure());
  EXPECT_TRUE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());
}

TEST_F(RunnerTest, EnabledBreakpoint)
{
  // Test preconditions
  Runner runner(empty_ui);
  EXPECT_TRUE(runner.IsFinished());  // empty procedure is finished by default
  EXPECT_FALSE(runner.IsRunning());

  // Set procedure and test conditions again
  EXPECT_NO_THROW(sync_proc->Setup());
  EXPECT_NO_THROW(runner.SetProcedure(sync_proc.get()));
  EXPECT_EQ(sync_proc->GetStatus(), ExecutionStatus::NOT_STARTED);
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());

  // Trying to set breakpoints at non-existing instructions
  EXPECT_THROW(runner.SetBreakpoint(nullptr), InvalidOperationException);
  auto instr = GlobalInstructionRegistry().Create("Wait");
  ASSERT_NE(instr.get(), nullptr);
  EXPECT_THROW(runner.SetBreakpoint(instr.get()), InvalidOperationException);

  // Set callback to pause runner and execute
  auto cb = [&runner](const Procedure&) { runner.Pause(); };
  runner.SetTickCallback(cb);
  EXPECT_NO_THROW(runner.ExecuteProcedure());
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());
  // Clear pause callback
  runner.SetTickCallback();

  // Fetch next instructions and set a breakpoint on the Inverter instruction
  auto next_instructions = GetNextInstructions(*sync_proc);
  EXPECT_EQ(next_instructions.size(), 3);
  auto predicate = [](const Instruction* instruction) {
    return instruction->GetType() == "Inverter";
  };
  auto it = std::find_if(next_instructions.begin(), next_instructions.end(), predicate);
  ASSERT_NE(it, next_instructions.end());
  EXPECT_NO_THROW(runner.SetBreakpoint(*it));

  // Check presence of one breakpoint that is set
  auto breakpoints = runner.GetBreakpoints();
  ASSERT_EQ(breakpoints.size(), 1);
  EXPECT_EQ(breakpoints[0].GetInstruction(), *it);
  EXPECT_EQ(breakpoints[0].GetStatus(), Breakpoint::kSet);
  EXPECT_NO_THROW(runner.ExecuteProcedure());
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());

  // Check presence of one breakpoint that is released
  breakpoints = runner.GetBreakpoints();
  ASSERT_EQ(breakpoints.size(), 1);
  EXPECT_EQ(breakpoints[0].GetInstruction(), *it);
  EXPECT_EQ(breakpoints[0].GetStatus(), Breakpoint::kReleased);

  // Query leaves
  auto next_leaves = GetNextLeaves(*sync_proc);
  ASSERT_EQ(next_leaves.size(), 1);
  auto found_it = std::find(next_instructions.begin(), next_instructions.end(), next_leaves[0]);
  EXPECT_NE(found_it, next_instructions.end());
  // Leave instruction is not the Inverter!
  EXPECT_NE(next_leaves[0], *it);

  // Next execute will pass the breakpoint
  EXPECT_NO_THROW(runner.ExecuteProcedure());
  EXPECT_TRUE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());
}

TEST_F(RunnerTest, DisabledBreakpoint)
{
  // Test preconditions
  Runner runner(empty_ui);
  EXPECT_TRUE(runner.IsFinished());  // empty procedure is finished by default
  EXPECT_FALSE(runner.IsRunning());

  // Set procedure and test conditions again
  EXPECT_NO_THROW(sync_proc->Setup());
  EXPECT_NO_THROW(runner.SetProcedure(sync_proc.get()));
  EXPECT_EQ(sync_proc->GetStatus(), ExecutionStatus::NOT_STARTED);
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());

  // Set callback to pause runner and execute
  auto cb = [&runner](const Procedure&) { runner.Pause(); };
  runner.SetTickCallback(cb);
  EXPECT_NO_THROW(runner.ExecuteProcedure());
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());
  // Clear pause callback
  runner.SetTickCallback();

  // Fetch next instructions and set a breakpoint on the Inverter instruction
  auto next_instructions = GetNextInstructions(*sync_proc);
  EXPECT_EQ(next_instructions.size(), 3);
  auto predicate = [](const Instruction* instruction) {
    return instruction->GetType() == "Inverter";
  };
  auto it = std::find_if(next_instructions.begin(), next_instructions.end(), predicate);
  ASSERT_NE(it, next_instructions.end());
  EXPECT_NO_THROW(runner.SetBreakpoint(*it));

  // Check presence of one breakpoint that is set
  auto breakpoints = runner.GetBreakpoints();
  ASSERT_EQ(breakpoints.size(), 1);
  EXPECT_EQ(breakpoints[0].GetInstruction(), *it);
  EXPECT_EQ(breakpoints[0].GetStatus(), Breakpoint::kSet);

  // Remove breakpoint and check
  EXPECT_NO_THROW(runner.RemoveBreakpoint(*it));
  breakpoints = runner.GetBreakpoints();
  ASSERT_EQ(breakpoints.size(), 0);

  // No more breakpoints, so the runner runs to completion
  EXPECT_NO_THROW(runner.ExecuteProcedure());
  EXPECT_TRUE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());
}

RunnerTest::RunnerTest()
    : mock_ui{}
    , empty_ui{}
    , async_proc{sup::sequencer::ParseProcedureString(AsyncProcedureString)}
    , sync_proc{sup::sequencer::ParseProcedureString(SyncProcedureString)}
    , copy_proc{sup::sequencer::ParseProcedureString(CopyVariableProcedureString)}
    , async_wait_proc{sup::sequencer::ParseProcedureString(AsyncWaitProcedureString)}
{
}

RunnerTest::~RunnerTest()
{
  if (async_proc)
  {
    async_proc->Reset(empty_ui);
  }
  if (sync_proc)
  {
    sync_proc->Reset(empty_ui);
  }
  if (copy_proc)
  {
    copy_proc->Reset(empty_ui);
  }
  if (async_wait_proc)
  {
    async_wait_proc->Reset(empty_ui);
  }
}
