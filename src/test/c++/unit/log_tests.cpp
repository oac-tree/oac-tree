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

#include "unit_test_helper.h"

#include <sup/sequencer/log/log_severity.h>

#include <sup/sequencer/log.h>

#include <gtest/gtest.h>

#include <sstream>

using namespace sup::sequencer;

const std::string TEST_FILE_NAME = "LogTestFile";

class LogTest : public ::testing::Test
{
protected:
  LogTest();
  virtual ~LogTest();
};

TEST_F(LogTest, Redirect)
{
  log::SetMaxSeverity(log::SUP_LOG_DEBUG);
  {
    std::ostringstream oss;
    log::LogStreamRedirector redirector(oss);
    std::string log_message = "logtest emergency";
    log::Emergency(log_message);
    EXPECT_NE(oss.str().find(log_message), std::string::npos);
    EXPECT_NE(oss.str().find(log::EmergencyString), std::string::npos);
  }
  {
    std::ostringstream oss;
    log::LogStreamRedirector redirector(oss);
    std::string log_message = "logtest alert";
    log::Alert(log_message);
    EXPECT_NE(oss.str().find(log_message), std::string::npos);
    EXPECT_NE(oss.str().find(log::AlertString), std::string::npos);
  }
  {
    std::ostringstream oss;
    log::LogStreamRedirector redirector(oss);
    std::string log_message = "logtest critical";
    log::Critical(log_message);
    EXPECT_NE(oss.str().find(log_message), std::string::npos);
    EXPECT_NE(oss.str().find(log::CriticalString), std::string::npos);
  }
  {
    std::ostringstream oss;
    log::LogStreamRedirector redirector(oss);
    std::string log_message = "logtest error";
    log::Error(log_message);
    EXPECT_NE(oss.str().find(log_message), std::string::npos);
    EXPECT_NE(oss.str().find(log::ErrorString), std::string::npos);
  }
  {
    std::ostringstream oss;
    log::LogStreamRedirector redirector(oss);
    std::string log_message = "logtest warning";
    log::Warning(log_message);
    EXPECT_NE(oss.str().find(log_message), std::string::npos);
    EXPECT_NE(oss.str().find(log::WarningString), std::string::npos);
  }
  {
    std::ostringstream oss;
    log::LogStreamRedirector redirector(oss);
    std::string log_message = "logtest notice";
    log::Notice(log_message);
    EXPECT_NE(oss.str().find(log_message), std::string::npos);
    EXPECT_NE(oss.str().find(log::NoticeString), std::string::npos);
  }
  {
    std::ostringstream oss;
    log::LogStreamRedirector redirector(oss);
    std::string log_message = "logtest info";
    log::Info(log_message);
    EXPECT_NE(oss.str().find(log_message), std::string::npos);
    EXPECT_NE(oss.str().find(log::InfoString), std::string::npos);
  }
  {
    std::ostringstream oss;
    log::LogStreamRedirector redirector(oss);
    std::string log_message = "logtest debug";
    log::Debug(log_message);
    EXPECT_TRUE(oss.str().empty());
  }
  {
    std::ostringstream oss;
    log::LogStreamRedirector redirector(oss);
    std::string log_message = "logtest trace";
    log::Trace(log_message);
    EXPECT_TRUE(oss.str().empty());
  }
}

LogTest::LogTest()
{}

LogTest::~LogTest() = default;
