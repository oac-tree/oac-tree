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

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/generic_utils.h>
#include <sup/sequencer/temp/user_input.h>

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

using namespace sup::sequencer;

/**
 * @brief TestUserInput handles only a single user input request for testing purposes.
 * GetUserValue() returns -1 for invalid or interrupted requests.
 *
 */
class TestUserInput : public IUserInput
{
public:
  TestUserInput(int value, unsigned int timeout_ms)
    : m_value{value}, m_timeout_ms{timeout_ms}, m_current_id{0}, m_halt{false} {}
  virtual ~TestUserInput() = default;

  int GetUserValue(sup::dto::uint64 id) override {
    if (m_current_id != 0)
    {
      return -1;
    }
    m_current_id = id;
    m_halt.store(false);
    auto finish = utils::GetNanosecsSinceEpoch() + m_timeout_ms * 1000000;
    while (!m_halt && finish > utils::GetNanosecsSinceEpoch())
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(DefaultSettings::TIMING_ACCURACY_MS));
    }
    m_current_id = 0;
    return m_halt ? -1 : m_value;
  }

  void Interrupt(sup::dto::uint64 id) override {
    if (id != m_current_id)
    {
      return;
    }
    m_halt.store(true);
  }

private:
  int m_value;
  unsigned int m_timeout_ms;
  sup::dto::uint64 m_current_id;
  std::atomic_bool m_halt;
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
  auto future = async_input.AddUserInputRequest();
  EXPECT_TRUE(future.IsValid());
  EXPECT_FALSE(future.IsReady());
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  EXPECT_TRUE(future.IsReady());
  EXPECT_EQ(future.GetValue(), 42);
  // After retrieval of user input, the request becomes invalid:
  EXPECT_FALSE(future.IsValid());
}

TEST_F(UserInputTest, Exceptions)
{
  TestUserInput user_input{42, 5000};
  AsyncUserInput async_input{user_input};
  auto future = async_input.AddUserInputRequest();
  ASSERT_TRUE(future.IsValid());
  EXPECT_FALSE(future.IsReady());
  EXPECT_THROW(future.GetValue(), InvalidOperationException);
  EXPECT_TRUE(future.IsValid());
  EXPECT_FALSE(future.IsReady());
}

TEST_F(UserInputTest, Move)
{
  TestUserInput user_input{42, 5000};
  AsyncUserInput async_input{user_input};
  auto future = async_input.AddUserInputRequest();
  EXPECT_TRUE(future.IsValid());
  EXPECT_FALSE(future.IsReady());
  AsyncUserInput::Future moved{std::move(future)};
  EXPECT_FALSE(future.IsValid());
  EXPECT_FALSE(future.IsReady());
  EXPECT_TRUE(moved.IsValid());
}

UserInputTest::UserInputTest() = default;

UserInputTest::~UserInputTest() = default;
