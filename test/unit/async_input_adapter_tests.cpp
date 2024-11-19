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
    : m_reply{reply}, m_timeout_ms{timeout_ms}, m_current_id{0}, m_halt{false}, m_n_requests{0} {}
  virtual ~TestUserInput() = default;

  UserInputReply GetUserValue(const UserInputRequest& request, sup::dto::uint64 id) {
    (void)request;
    UserInputReply failure{ InputRequestType::kUserValue, false, {} };
    if (m_current_id != 0)
    {
      return failure;
    }
    ++m_n_requests;
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

  int GetNumberOfRequests() const { return m_n_requests; }

private:
  UserInputReply m_reply;
  unsigned int m_timeout_ms;
  sup::dto::uint64 m_current_id;
  std::atomic_bool m_halt;
  int m_n_requests;
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
  EXPECT_EQ(user_input.GetNumberOfRequests(), 1);
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
  EXPECT_TRUE(future->IsValid());
  EXPECT_FALSE(future->IsReady());
  EXPECT_THROW(future->GetValue(), InvalidOperationException);
  EXPECT_TRUE(future->IsValid());
  EXPECT_FALSE(future->IsReady());
}

TEST_F(AsyncInputAdapterTest, FutureDestruction)
{
  // Verify that the returned future does not join the created thread during destruction
  TestUserInput user_input{m_reply, 5000};
  AsyncInputAdapter async_input{ GetInputFunction(user_input), GetInterruptFunction(user_input)};
  auto start = std::chrono::high_resolution_clock::now();
  {
    auto future = async_input.AddUserInputRequest(m_request);
    EXPECT_TRUE(future->IsValid());
    EXPECT_FALSE(future->IsReady());
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  EXPECT_TRUE(duration_ms.count() < 2000);
}

TEST_F(AsyncInputAdapterTest, MultipleRequests)
{
  // Verify that a request that is cancelled before being handled works as expected (it is
  // only removed from the internal queue).
  TestUserInput user_input{m_reply, 500};
  AsyncInputAdapter async_input{ GetInputFunction(user_input), GetInterruptFunction(user_input)};
  auto future_1 = async_input.AddUserInputRequest(m_request);
  EXPECT_TRUE(future_1->IsValid());
  EXPECT_FALSE(future_1->IsReady());
  {
    // This request will be cancelled before it has a chance of being handled, so number of
    // requests will be only one.
    auto future_2 = async_input.AddUserInputRequest(m_request);
    EXPECT_TRUE(future_2->IsValid());
    EXPECT_FALSE(future_2->IsReady());
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  EXPECT_EQ(user_input.GetNumberOfRequests(), 1);
  EXPECT_TRUE(future_1->IsReady());
  EXPECT_EQ(future_1->GetValue(), m_reply);
}

AsyncInputAdapterTest::AsyncInputAdapterTest()
  : m_value{sup::dto::UnsignedInteger8Type, 42u}
  , m_request{CreateUserValueRequest(m_value, "Give me a number")}
  , m_reply{CreateUserValueReply(true, m_value)}
{}

AsyncInputAdapterTest::~AsyncInputAdapterTest() = default;
