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

#include <sup/sequencer/temp/user_input.h>

#include <gtest/gtest.h>

#include <chrono>
#include <thread>

using namespace sup::sequencer;

class TestUserInput : public IUserInput
{
public:
  TestUserInput(int value, unsigned int timeout_ms)
    : m_value{value}, m_timeout_ms{timeout_ms} {}
  virtual ~TestUserInput() = default;

  int GetUserValue() const override {
    std::this_thread::sleep_for(std::chrono::milliseconds(m_timeout_ms));
    return m_value;
  }

private:
  int m_value;
  unsigned int m_timeout_ms;
};

class UserInputTest : public ::testing::Test
{
protected:
  UserInputTest();
  virtual ~UserInputTest();
};

TEST_F(UserInputTest, Construction)
{
  TestUserInput user_input{42, 500};
  AsyncUserInput async_input{user_input};
  EXPECT_TRUE(async_input.AddUserInputRequest());
  EXPECT_FALSE(async_input.UserInputRequestReady());
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  EXPECT_TRUE(async_input.UserInputRequestReady());
  EXPECT_EQ(async_input.GetUserInput(), 42);
}

UserInputTest::UserInputTest() = default;

UserInputTest::~UserInputTest() = default;
