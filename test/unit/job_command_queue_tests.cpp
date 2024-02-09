/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/sequencer/job_command_queue.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

class JobCommandQueueTest : public ::testing::Test
{
protected:
  JobCommandQueueTest() = default;
  virtual ~JobCommandQueueTest() = default;

};

TEST_F(JobCommandQueueTest, PushPop)
{
  JobCommandQueue queue;
  EXPECT_TRUE(queue.IsEmpty());
  JobCommand command = JobCommand::kStart;
  EXPECT_FALSE(queue.TryPop(command));
  queue.Push(JobCommand::kPause);
  EXPECT_FALSE(queue.IsEmpty());
  EXPECT_TRUE(queue.TryPop(command));
  EXPECT_EQ(command, JobCommand::kPause);
  queue.Push(JobCommand::kHalt);
  command = queue.WaitForNextCommand();
  EXPECT_EQ(command, JobCommand::kHalt);
}

TEST_F(JobCommandQueueTest, PriorityPush)
{
  JobCommandQueue queue;
  EXPECT_TRUE(queue.IsEmpty());
  queue.Push(JobCommand::kPause);
  EXPECT_EQ(queue.GetSize(), 1);
  queue.PriorityPush(JobCommand::kHalt, {});
  EXPECT_EQ(queue.GetSize(), 2);
  queue.PriorityPush(JobCommand::kHalt, {});
  EXPECT_EQ(queue.GetSize(), 2);
  queue.PriorityPush(JobCommand::kTerminate, {});
  EXPECT_EQ(queue.GetSize(), 3);
  queue.PriorityPush(JobCommand::kHalt, {});
  EXPECT_EQ(queue.GetSize(), 3);
  queue.PriorityPush(JobCommand::kReset, {});
  EXPECT_EQ(queue.GetSize(), 3);

  ASSERT_FALSE(queue.IsEmpty());
  auto command = queue.WaitForNextCommand();
  EXPECT_EQ(command, JobCommand::kTerminate);
  ASSERT_FALSE(queue.IsEmpty());
  command = queue.WaitForNextCommand();
  EXPECT_EQ(command, JobCommand::kHalt);
  ASSERT_FALSE(queue.IsEmpty());
  command = queue.WaitForNextCommand();
  EXPECT_EQ(command, JobCommand::kPause);
  EXPECT_TRUE(queue.IsEmpty());
}
