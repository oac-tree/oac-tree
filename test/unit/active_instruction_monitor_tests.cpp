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
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include <sup/oac-tree/active_instruction_monitor.h>
#include <sup/oac-tree/instruction_filters.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

class ActiveInstructionMonitorTest : public ::testing::Test
{
protected:
  ActiveInstructionMonitorTest();
  virtual ~ActiveInstructionMonitorTest() = default;

  ActiveInstructionMonitor::ActiveInstructionsChanged GetCallback();

  std::size_t m_count;
  std::set<sup::dto::uint32> m_active_instr;
};

TEST_F(ActiveInstructionMonitorTest, Construction)
{
  ActiveInstructionMonitor monitor{GetCallback()};
  EXPECT_EQ(m_count, 0);
  EXPECT_TRUE(m_active_instr.empty());
}

TEST_F(ActiveInstructionMonitorTest, InactiveStates)
{
  ActiveInstructionMonitor monitor{GetCallback()};

  // Test with NOT_STARTED state
  monitor.InstructionStatusUpdated(1, ExecutionStatus::NOT_STARTED);
  EXPECT_EQ(m_count, 0);
  EXPECT_TRUE(m_active_instr.empty());

  // Test with SUCCESS state
  monitor.InstructionStatusUpdated(2, ExecutionStatus::SUCCESS);
  EXPECT_EQ(m_count, 0);
  EXPECT_TRUE(m_active_instr.empty());

  // Test with FAILURE state
  monitor.InstructionStatusUpdated(0, ExecutionStatus::FAILURE);
  EXPECT_EQ(m_count, 0);
  EXPECT_TRUE(m_active_instr.empty());
}

TEST_F(ActiveInstructionMonitorTest, ActiveStates)
{
  ActiveInstructionMonitor monitor{GetCallback()};

  // Test with NOT_FINISHED state
  monitor.InstructionStatusUpdated(1, ExecutionStatus::NOT_FINISHED);
  EXPECT_EQ(m_count, 1);
  EXPECT_EQ(m_active_instr.size(), 1);
  EXPECT_TRUE(m_active_instr.find(1) != m_active_instr.end());

  // Test with RUNNING state
  monitor.InstructionStatusUpdated(0, ExecutionStatus::RUNNING);
  EXPECT_EQ(m_count, 2);
  EXPECT_EQ(m_active_instr.size(), 2);
  EXPECT_TRUE(m_active_instr.find(0) != m_active_instr.end());
}

TEST_F(ActiveInstructionMonitorTest, OnlyNotifyOnUpdatedList)
{
  ActiveInstructionMonitor monitor{GetCallback()};

  // Add some active instructions
  monitor.InstructionStatusUpdated(0, ExecutionStatus::NOT_FINISHED);
  monitor.InstructionStatusUpdated(4, ExecutionStatus::RUNNING);
  monitor.InstructionStatusUpdated(5, ExecutionStatus::RUNNING);
  EXPECT_EQ(m_count, 3);
  EXPECT_EQ(m_active_instr.size(), 3);
  EXPECT_TRUE(m_active_instr.find(0) != m_active_instr.end());
  EXPECT_TRUE(m_active_instr.find(4) != m_active_instr.end());
  EXPECT_TRUE(m_active_instr.find(5) != m_active_instr.end());

  // No callback called when instruction switches between two active states
  monitor.InstructionStatusUpdated(0, ExecutionStatus::RUNNING);
  monitor.InstructionStatusUpdated(4, ExecutionStatus::NOT_FINISHED);
  monitor.InstructionStatusUpdated(5, ExecutionStatus::NOT_FINISHED);
  EXPECT_EQ(m_count, 3);
  EXPECT_EQ(m_active_instr.size(), 3);
  EXPECT_TRUE(m_active_instr.find(0) != m_active_instr.end());
  EXPECT_TRUE(m_active_instr.find(4) != m_active_instr.end());
  EXPECT_TRUE(m_active_instr.find(5) != m_active_instr.end());

  // No callback called when instruction switched between two inactive states
  monitor.InstructionStatusUpdated(1, ExecutionStatus::SUCCESS);
  monitor.InstructionStatusUpdated(2, ExecutionStatus::NOT_STARTED);
  monitor.InstructionStatusUpdated(3, ExecutionStatus::FAILURE);
  EXPECT_EQ(m_count, 3);
  EXPECT_EQ(m_active_instr.size(), 3);
  EXPECT_TRUE(m_active_instr.find(0) != m_active_instr.end());
  EXPECT_TRUE(m_active_instr.find(4) != m_active_instr.end());
  EXPECT_TRUE(m_active_instr.find(5) != m_active_instr.end());

  // Instruction is removed if it goes from active to inactive
  monitor.InstructionStatusUpdated(0, ExecutionStatus::SUCCESS);
  monitor.InstructionStatusUpdated(5, ExecutionStatus::NOT_STARTED);
  EXPECT_EQ(m_count, 5);
  EXPECT_EQ(m_active_instr.size(), 1);
  EXPECT_TRUE(m_active_instr.find(4) != m_active_instr.end());

  // Instruction is added if it goes from inactive to active
  monitor.InstructionStatusUpdated(3, ExecutionStatus::RUNNING);
  monitor.InstructionStatusUpdated(5, ExecutionStatus::NOT_FINISHED);
  EXPECT_EQ(m_count, 7);
  EXPECT_EQ(m_active_instr.size(), 3);
  EXPECT_TRUE(m_active_instr.find(3) != m_active_instr.end());
  EXPECT_TRUE(m_active_instr.find(4) != m_active_instr.end());
  EXPECT_TRUE(m_active_instr.find(5) != m_active_instr.end());
}

TEST_F(ActiveInstructionMonitorTest, WithActiveAncestorFilter)
{
  // This list of parent indices represents the following tree:
  // 0
  // ├── 3
  // │   ├── 2
  // │   └── 1
  // └── 4
  const std::vector<sup::dto::uint32> parent_indices = { kIsRootIndex, 3, 3, 0, 0 };
  auto filter = AncestorsActiveFilter{parent_indices};
  ActiveInstructionMonitor monitor{GetCallback(), filter};

  // Activate instruction 1, which has no active ancestors
  monitor.InstructionStatusUpdated(1, ExecutionStatus::NOT_FINISHED);
  EXPECT_EQ(m_count, 1);
  EXPECT_EQ(m_active_instr.size(), 0);
  // Active its grandparent, which will only activate the root
  monitor.InstructionStatusUpdated(0, ExecutionStatus::RUNNING);
  EXPECT_EQ(m_count, 2);
  EXPECT_EQ(m_active_instr.size(), 1);
  EXPECT_TRUE(m_active_instr.find(0) != m_active_instr.end());
  // Active node 3, which will cause node 1 to be reported as active
  monitor.InstructionStatusUpdated(3, ExecutionStatus::RUNNING);
  EXPECT_EQ(m_count, 3);
  EXPECT_EQ(m_active_instr.size(), 3);
  EXPECT_TRUE(m_active_instr.find(0) != m_active_instr.end());
  EXPECT_TRUE(m_active_instr.find(1) != m_active_instr.end());
  EXPECT_TRUE(m_active_instr.find(3) != m_active_instr.end());
  // Deactivate the root node, which will deactivate all nodes
  monitor.InstructionStatusUpdated(0, ExecutionStatus::SUCCESS);
  EXPECT_EQ(m_count, 4);
  EXPECT_EQ(m_active_instr.size(), 0);
}


ActiveInstructionMonitorTest::ActiveInstructionMonitorTest()
  : m_count{0}
  , m_active_instr{}
{}

ActiveInstructionMonitor::ActiveInstructionsChanged ActiveInstructionMonitorTest::GetCallback()
{
  return [this](const std::set<sup::dto::uint32>& update)
    {
      m_count++;
      m_active_instr = update;
    };
}
