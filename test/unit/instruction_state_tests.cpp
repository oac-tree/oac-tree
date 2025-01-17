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

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_state.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

class InstructionStateTest : public ::testing::Test
{
protected:
  InstructionStateTest();
  virtual ~InstructionStateTest();
};

TEST_F(InstructionStateTest, Comparison)
{
  {
    // Equal states
    InstructionState state_1{true, ExecutionStatus::NOT_FINISHED};
    InstructionState state_2{true, ExecutionStatus::NOT_FINISHED};
    EXPECT_EQ(state_1, state_2);
    EXPECT_TRUE(state_1 == state_2);
    EXPECT_FALSE(state_1 != state_2);
  }
  {
    // Different breakpoint state
    InstructionState state_1{true, ExecutionStatus::RUNNING};
    InstructionState state_2{false, ExecutionStatus::RUNNING};
    EXPECT_NE(state_1, state_2);
    EXPECT_FALSE(state_1 == state_2);
    EXPECT_TRUE(state_1 != state_2);
  }
  {
    // Different execution status
    InstructionState state_1{false, ExecutionStatus::NOT_STARTED};
    InstructionState state_2{false, ExecutionStatus::SUCCESS};
    EXPECT_NE(state_1, state_2);
    EXPECT_FALSE(state_1 == state_2);
    EXPECT_TRUE(state_1 != state_2);
  }
}

TEST_F(InstructionStateTest, ToFromAnyValue)
{
  {
    // Encode to AnyValue and back
    InstructionState state{true, ExecutionStatus::FAILURE};
    auto state_av = ToAnyValue(state);
    auto read_back = ToInstructionState(state_av);
    EXPECT_EQ(read_back, state);
  }
  {
    // Piecewise copy of AnyValue to ensure member fields
    InstructionState state{false, ExecutionStatus::SUCCESS};
    auto state_av = ToAnyValue(state);
    auto new_state_av = sup::dto::EmptyStruct(Constants::kInstructionStateType);
    new_state_av.AddMember(Constants::kExecStatusField, state_av[Constants::kExecStatusField]);
    new_state_av.AddMember(Constants::kBreakpointField, state_av[Constants::kBreakpointField]);
    auto read_back = ToInstructionState(new_state_av);
    EXPECT_EQ(read_back, state);
  }
  {
    // Missing breakpoint state
    InstructionState state{false, ExecutionStatus::SUCCESS};
    auto state_av = ToAnyValue(state);
    auto new_state_av = sup::dto::EmptyStruct(Constants::kInstructionStateType);
    new_state_av.AddMember(Constants::kExecStatusField, state_av[Constants::kExecStatusField]);
    EXPECT_THROW(ToInstructionState(new_state_av), InvalidOperationException);
  }
  {
    // Wrong type of breakpoint state
    InstructionState state{false, ExecutionStatus::SUCCESS};
    auto state_av = ToAnyValue(state);
    auto new_state_av = sup::dto::EmptyStruct(Constants::kInstructionStateType);
    new_state_av.AddMember(Constants::kExecStatusField, state_av[Constants::kExecStatusField]);
    new_state_av.AddMember(Constants::kBreakpointField, "should_be_a_boolean");
    EXPECT_THROW(ToInstructionState(new_state_av), InvalidOperationException);
  }
  {
    // Missing execution status
    InstructionState state{false, ExecutionStatus::SUCCESS};
    auto state_av = ToAnyValue(state);
    auto new_state_av = sup::dto::EmptyStruct(Constants::kInstructionStateType);
    new_state_av.AddMember(Constants::kBreakpointField, state_av[Constants::kBreakpointField]);
    EXPECT_THROW(ToInstructionState(new_state_av), InvalidOperationException);
  }
  {
    // Wrong type of execution status
    InstructionState state{false, ExecutionStatus::SUCCESS};
    auto state_av = ToAnyValue(state);
    auto new_state_av = sup::dto::EmptyStruct(Constants::kInstructionStateType);
    new_state_av.AddMember(Constants::kExecStatusField, { sup::dto::SignedInteger64Type, 42 });
    new_state_av.AddMember(Constants::kBreakpointField, state_av[Constants::kBreakpointField]);
    EXPECT_THROW(ToInstructionState(new_state_av), InvalidOperationException);
  }
}

InstructionStateTest::InstructionStateTest() = default;

InstructionStateTest::~InstructionStateTest() = default;
