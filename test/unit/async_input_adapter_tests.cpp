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

#include <sup/sequencer/async_input_adapter.h>
#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/generic_utils.h>

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

using namespace sup::sequencer;
using namespace std::placeholders;

/**
 * @brief TestUserInput handles only a single user input request for testing purposes.
 * GetUserValue() returns -1 for invalid or interrupted requests.
 *
 */
class TestUserInput
{
public:
  TestUserInput(UserInputReply reply, unsigned int timeout_ms)
    : m_reply{reply}, m_timeout_ms{timeout_ms}, m_current_id{0}, m_halt{false} {}
  virtual ~TestUserInput() = default;

  UserInputReply GetUserValue(const UserInputRequest& request, sup::dto::uint64 id) {
    (void)request;
    UserInputReply failure{ InputRequestType::kUserValue, false, {} };
    if (m_current_id != 0)
    {
      return failure;
    }
    m_current_id = id;
    m_halt.store(false);
    auto finish = utils::GetNanosecsSinceEpoch() + m_timeout_ms * 1000000;
    while (!m_halt && finish > utils::GetNanosecsSinceEpoch())
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(DefaultSettings::TIMING_ACCURACY_MS));
    }
    m_current_id = 0;
    return m_halt ? failure : m_reply;
  }

  void Interrupt(sup::dto::uint64 id) {
    if (id != m_current_id)
    {
      return;
    }
    m_halt.store(true);
  }

private:
  UserInputReply m_reply;
  unsigned int m_timeout_ms;
  sup::dto::uint64 m_current_id;
  std::atomic_bool m_halt;
};

inline AsyncInputAdapter::InputFunction GetInputFunction(TestUserInput& test_input)
{
  return std::bind(&TestUserInput::GetUserValue, std::addressof(test_input), _1, _2);
}

inline AsyncInputAdapter::InterruptFunction GetInterruptFunction(TestUserInput& test_input)
{
  return std::bind(&TestUserInput::Interrupt, std::addressof(test_input), _1);
}

class AsyncInputAdapterTest : public ::testing::Test
{
protected:
  AsyncInputAdapterTest();
  virtual ~AsyncInputAdapterTest();
  sup::dto::AnyValue m_value;
  UserInputRequest m_request;
  UserInputReply m_reply;
};

TEST_F(AsyncInputAdapterTest, Construction)
{
  TestUserInput user_input{m_reply, 500};
  AsyncInputAdapter async_input{ GetInputFunction(user_input), GetInterruptFunction(user_input)};
  auto future = async_input.AddUserInputRequest(m_request);
  EXPECT_TRUE(future->IsValid());
  EXPECT_FALSE(future->IsReady());
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  EXPECT_TRUE(future->IsReady());
  EXPECT_EQ(future->GetValue(), m_reply);
  // After retrieval of user input, the request becomes invalid:
  EXPECT_FALSE(future->IsValid());
}

TEST_F(AsyncInputAdapterTest, Exceptions)
{
  TestUserInput user_input{m_reply, 5000};
  AsyncInputAdapter async_input{ GetInputFunction(user_input), GetInterruptFunction(user_input)};
  auto future = async_input.AddUserInputRequest(m_request);
  ASSERT_TRUE(future->IsValid());
  EXPECT_FALSE(future->IsReady());
  EXPECT_THROW(future->GetValue(), InvalidOperationException);
  EXPECT_TRUE(future->IsValid());
  EXPECT_FALSE(future->IsReady());
}

AsyncInputAdapterTest::AsyncInputAdapterTest()
  : m_value{sup::dto::UnsignedInteger8Type, 42u}
  , m_request{CreateUserValueRequest(m_value, "Give me a number")}
  , m_reply{CreateUserValueReply(true, m_value)}
{}

AsyncInputAdapterTest::~AsyncInputAdapterTest() = default;
