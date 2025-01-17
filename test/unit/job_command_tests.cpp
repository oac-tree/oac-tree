/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
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

#include <sup/oac-tree/job_commands.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

class JobCommandTest : public ::testing::Test
{
protected:
  JobCommandTest() = default;
  virtual ~JobCommandTest() = default;

};

TEST_F(JobCommandTest, Comparison)
{
  EXPECT_LT(JobCommand::kStart, JobCommand::kStep);
  EXPECT_LT(JobCommand::kStart, JobCommand::kPause);
  EXPECT_LT(JobCommand::kPause, JobCommand::kReset);
  EXPECT_LT(JobCommand::kReset, JobCommand::kHalt);
  EXPECT_LT(JobCommand::kHalt, JobCommand::kTerminate);
  EXPECT_FALSE(JobCommand::kTerminate <= JobCommand::kHalt);
  EXPECT_FALSE(JobCommand::kHalt <= JobCommand::kPause);
}
