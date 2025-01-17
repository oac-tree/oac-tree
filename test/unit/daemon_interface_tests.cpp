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

#include <app/oac-tree-daemon/daemon_interface.h>

#include <sup/oac-tree/log_severity.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>

#include <sup/log/default_loggers.h>

#include <gtest/gtest.h>

#include <memory>
#include <sstream>

using namespace sup::oac_tree;

class DaemonInterfaceTest : public ::testing::Test
{
public:
  using LogEntry = std::tuple<int, std::string, std::string>;
protected:

  DaemonInterfaceTest();
  virtual ~DaemonInterfaceTest();

  std::vector<LogEntry> m_log_entries;
  DaemonInterface daemon_interface;
  DaemonInterface daemon_interface_logging;
  std::unique_ptr<Instruction> wait;
  std::ostringstream out_stream;
};

TEST_F(DaemonInterfaceTest, UpdateInstructionStatus)
{
  EXPECT_TRUE(m_log_entries.empty());
  daemon_interface.UpdateInstructionStatus(wait.get());
  EXPECT_TRUE(m_log_entries.empty());
}

TEST_F(DaemonInterfaceTest, UpdateInstructionStatusLogged)
{
  EXPECT_TRUE(m_log_entries.empty());
  daemon_interface_logging.UpdateInstructionStatus(wait.get());
  ASSERT_FALSE(m_log_entries.empty());
  auto last_message = std::get<2>(m_log_entries.back());
  EXPECT_NE(last_message.find(StatusToString(ExecutionStatus::NOT_STARTED)), std::string::npos);
}

TEST_F(DaemonInterfaceTest, GetUserValue)
{
  EXPECT_TRUE(m_log_entries.empty());
  sup::dto::AnyValue val(sup::dto::UnsignedInteger32Type, 1234);
  auto [retrieved, _] = GetBlockingUserValue(daemon_interface, val, "");
  EXPECT_FALSE(retrieved);
  EXPECT_FALSE(m_log_entries.empty());
}

TEST_F(DaemonInterfaceTest, GetUserChoice)
{
  EXPECT_TRUE(m_log_entries.empty());
  auto options = std::vector<std::string>({"one", "two"});
  auto [retrieved, _] = GetBlockingUserChoice(daemon_interface, options, {});
  EXPECT_FALSE(retrieved);
  EXPECT_FALSE(m_log_entries.empty());
}

TEST_F(DaemonInterfaceTest, PutValue)
{
  EXPECT_TRUE(m_log_entries.empty());
  sup::dto::AnyValue val = 23;
  EXPECT_TRUE(daemon_interface_logging.PutValue(val, ""));
  ASSERT_FALSE(m_log_entries.empty());
  auto last_message = std::get<2>(m_log_entries.back());
  EXPECT_NE(last_message.find("23"), std::string::npos);
}

TEST_F(DaemonInterfaceTest, Message)
{
  EXPECT_TRUE(m_log_entries.empty());
  std::string message = "Hello message";
  EXPECT_NO_THROW(daemon_interface_logging.Message(message));
  ASSERT_FALSE(m_log_entries.empty());
  auto last_message = std::get<2>(m_log_entries.back());
  EXPECT_NE(last_message.find(message), std::string::npos);
}

static sup::log::DefaultLogger CreateDefaultTestLogger(
  int severity, std::vector<DaemonInterfaceTest::LogEntry>* log_entries)
{
  auto logger = sup::log::DefaultLogger([log_entries](int severity, const std::string& source,
                                                      const std::string& message)
                                          {
                                            log_entries->emplace_back(severity, source, message);
                                          },
                                        "DaemonInterfaceTest", severity);
  return logger;
}

DaemonInterfaceTest::DaemonInterfaceTest()
    : m_log_entries{}
    , daemon_interface{CreateDefaultTestLogger(log::SUP_SEQ_LOG_ERR, &m_log_entries)}
    , daemon_interface_logging{CreateDefaultTestLogger(log::SUP_SEQ_LOG_INFO, &m_log_entries)}
    , wait{GlobalInstructionRegistry().Create("Wait")}
    , out_stream{}
{}

DaemonInterfaceTest::~DaemonInterfaceTest() = default;
