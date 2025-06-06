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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <app/oac-tree-cli/cl_interface.h>

#include "std_redirectors.h"

#include <sup/oac-tree/log_severity.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>

#include <sup/log/basic_logger.h>

#include <gtest/gtest.h>

#include <cstring>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>

using namespace sup::oac_tree;

class CLInterfaceTest : public ::testing::Test
{
protected:
  using LogEntry = std::tuple<int, std::string, std::string>;

  CLInterfaceTest();
  virtual ~CLInterfaceTest();

  std::vector<LogEntry> m_log_entries;
  CLInterface cli;
  CLInterface cli_verbose;
  std::unique_ptr<Instruction> wait;
};

TEST_F(CLInterfaceTest, UpdateInstructionStatus)
{
  EXPECT_TRUE(m_log_entries.empty());
  cli.UpdateInstructionStatus(wait.get());
  EXPECT_TRUE(m_log_entries.empty());
  cli_verbose.UpdateInstructionStatus(wait.get());
  EXPECT_FALSE(m_log_entries.empty());
}

TEST_F(CLInterfaceTest, VariableUpdated)
{
  sup::dto::AnyValue val(sup::dto::UnsignedInteger32Type, 1234);
  std::string name = "TestUpdated";
  EXPECT_TRUE(m_log_entries.empty());
  cli.VariableUpdated(name, val, true);
  EXPECT_TRUE(m_log_entries.empty());
  cli_verbose.VariableUpdated(name, val, true);
  EXPECT_FALSE(m_log_entries.empty());
}

TEST_F(CLInterfaceTest, GetUserValue)
{
  sup::dto::AnyValue val(sup::dto::UnsignedInteger32Type);
  std::istringstream input("567890");
  CinRedirector redirect(input);
  auto [retrieved, value] = GetBlockingUserValue(cli, val, "");
  EXPECT_TRUE(retrieved);
  auto result = value.As<sup::dto::uint32>();
  EXPECT_EQ(result, 567890u);
}

TEST_F(CLInterfaceTest, GetUserValueBool)
{
  sup::dto::AnyValue val(sup::dto::BooleanType);
  std::istringstream input("true");
  CinRedirector redirect(input);
  auto [retrieved, value] = GetBlockingUserValue(cli, val, "");
  EXPECT_TRUE(retrieved);
  auto result = value.As<sup::dto::boolean>();
  EXPECT_EQ(result, true);
}

TEST_F(CLInterfaceTest, GetUserValueString)
{
  sup::dto::AnyValue val(sup::dto::StringType);
  std::istringstream input("Test string");
  CinRedirector redirect(input);
  auto [retrieved, value] = GetBlockingUserValue(cli, val, "");
  EXPECT_TRUE(retrieved);
  auto val_str = value.As<std::string>();
  EXPECT_TRUE(val_str == "Test string");
}

TEST_F(CLInterfaceTest, GetUserValueParseError)
{
  EXPECT_TRUE(m_log_entries.empty());
  {
    sup::dto::AnyValue val(sup::dto::UnsignedInteger32Type);
    std::istringstream input("twenty-four");
    CinRedirector redirect(input);
    auto [retrieved, _] = GetBlockingUserValue(cli, val, "");
    EXPECT_FALSE(retrieved);
    EXPECT_EQ(m_log_entries.size(), 1);
  }
  {
    sup::dto::AnyValue val(sup::dto::BooleanType);
    std::istringstream input("nottrue");
    CinRedirector redirect(input);
    auto [retrieved, _] = GetBlockingUserValue(cli, val, "");
    EXPECT_FALSE(retrieved);
    EXPECT_EQ(m_log_entries.size(), 2);
  }
}

TEST_F(CLInterfaceTest, GetUserValueUnsupportedType)
{
  EXPECT_TRUE(m_log_entries.empty());
  sup::dto::AnyType test_type{{
    {"timestamp", sup::dto::UnsignedInteger64Type},
    {"count", sup::dto::UnsignedInteger16Type}
  }, "CompoundTestType"};
  sup::dto::AnyValue val(test_type);
  std::istringstream input("17");
  CinRedirector redirect(input);
  auto [retrieved, _] = GetBlockingUserValue(cli, val, "");
  EXPECT_FALSE(retrieved);
  EXPECT_EQ(m_log_entries.size(), 1);
}

TEST_F(CLInterfaceTest, GetUserChoice)
{
  auto options = std::vector<std::string>({"one", "two"});
  std::istringstream input("2");
  CinRedirector redirect(input);
  auto [retrieved, value] = GetBlockingUserChoice(cli, options, {});
  EXPECT_TRUE(retrieved);
  // CLI interface uses one-based indexing, but the options vector has zero-based indexing, so
  // returned integer is one smaller than the string input:
  EXPECT_EQ(value, 1);
}

TEST_F(CLInterfaceTest, GetUserChoiceParseError)
{
  EXPECT_TRUE(m_log_entries.empty());
  auto options = std::vector<std::string>({"one", "two"});
  std::istringstream input("one");
  CinRedirector redirect(input);
  auto [retrieved, value] = GetBlockingUserChoice(cli, options, {});
  EXPECT_FALSE(retrieved);
  EXPECT_EQ(value, -1);
  EXPECT_EQ(m_log_entries.size(), 1);
}

TEST_F(CLInterfaceTest, GetUserChoiceOutOfBounds)
{
  EXPECT_TRUE(m_log_entries.empty());
  auto options = std::vector<std::string>({"one", "two"});
  std::istringstream input("3");
  CinRedirector redirect(input);
  auto [retrieved, value] = GetBlockingUserChoice(cli, options, {});
  EXPECT_FALSE(retrieved);
  EXPECT_EQ(value, -1);
  EXPECT_EQ(m_log_entries.size(), 1);
}

TEST_F(CLInterfaceTest, PutValue)
{
  sup::dto::AnyValue val = 23;
  std::ostringstream oss;
  CoutRedirector redirect(oss);
  EXPECT_TRUE(cli.PutValue(val, ""));
  EXPECT_NE(oss.str().find("23"), std::string::npos);
}

TEST_F(CLInterfaceTest, Message)
{
  std::string message = "Hello message";
  std::ostringstream oss;
  CoutRedirector redirect(oss);
  EXPECT_NO_THROW(cli.Message(message));
  EXPECT_NE(oss.str().find(message), std::string::npos);
}

CLInterfaceTest::CLInterfaceTest()
    : m_log_entries{}
    , cli{sup::log::BasicLogger([this](int severity, const std::string& source,
                                  const std::string& message)
                                  {
                                   m_log_entries.emplace_back(severity, source, message);
                                  },
                                "CLInterfaceTest", log::SUP_SEQ_LOG_WARNING)}
    , cli_verbose{sup::log::BasicLogger([this](int severity, const std::string& source,
                                          const std::string& message)
                                          {
                                           m_log_entries.emplace_back(severity, source, message);
                                          },
                                        "CLInterfaceTest", log::SUP_SEQ_LOG_INFO)}
    , wait{GlobalInstructionRegistry().Create("Wait")}
{}

CLInterfaceTest::~CLInterfaceTest() {}
