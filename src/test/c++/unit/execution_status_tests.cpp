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

#include <sup/sequencer/execution_status.h>

#include <sup/sequencer/log.h>

#include <gtest/gtest.h>

#include <set>

using namespace sup::sequencer;

TEST(ExecutionStatus, EnumeratorStrings)
{
  // No user code should depend on the exact string representations of the execution
  // statuses. So this test only assures they are not empty and different.
  std::set<std::string> status_strings;
  auto status_str = StatusToString(ExecutionStatus::NOT_STARTED);
  EXPECT_FALSE(status_str.empty());
  auto insert_result = status_strings.insert(status_str);
  EXPECT_TRUE(insert_result.second);
  status_str = StatusToString(ExecutionStatus::NOT_FINISHED);
  EXPECT_FALSE(status_str.empty());
  insert_result = status_strings.insert(status_str);
  EXPECT_TRUE(insert_result.second);
  status_str = StatusToString(ExecutionStatus::RUNNING);
  EXPECT_FALSE(status_str.empty());
  insert_result = status_strings.insert(status_str);
  EXPECT_TRUE(insert_result.second);
  status_str = StatusToString(ExecutionStatus::SUCCESS);
  EXPECT_FALSE(status_str.empty());
  insert_result = status_strings.insert(status_str);
  EXPECT_TRUE(insert_result.second);
  status_str = StatusToString(ExecutionStatus::FAILURE);
  EXPECT_FALSE(status_str.empty());
  insert_result = status_strings.insert(status_str);
  EXPECT_TRUE(insert_result.second);
  EXPECT_EQ(status_strings.size(), 5u) << "All existing ExecutionStatus enumerators should "
                                          "have different string representations.";

  // Test undefined enumeration values
  status_str = StatusToString(static_cast<ExecutionStatus>(42));
  EXPECT_FALSE(status_str.empty());
  insert_result = status_strings.insert(status_str);
  EXPECT_TRUE(insert_result.second);
  EXPECT_EQ(status_strings.size(), 6u);
}
