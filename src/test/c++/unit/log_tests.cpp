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

#include <sup/sequencer/log_severity.h>

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
  // Test debug and trace functions through different interface since they can be disabled
  {
    std::ostringstream oss;
    log::LogStreamRedirector redirector(oss);
    std::string log_message = "logtest debug";
    log::SimpleDebug("LogTest", log_message);
    EXPECT_NE(oss.str().find(log_message), std::string::npos);
    EXPECT_NE(oss.str().find(log::DebugString), std::string::npos);
  }
  {
    std::ostringstream oss;
    log::LogStreamRedirector redirector(oss);
    std::string log_message = "logtest trace";
    log::SimpleTrace("LogTest", log_message);
    EXPECT_NE(oss.str().find(log_message), std::string::npos);
    EXPECT_NE(oss.str().find(log::TraceString), std::string::npos);
  }
}

TEST_F(LogTest, StdOut)
{
  log::SetStdOut();
  EXPECT_NO_THROW(log::Emergency("To standard output"));
  EXPECT_NO_THROW(log::Alert("To standard output"));
  EXPECT_NO_THROW(log::Critical("To standard output"));
  EXPECT_NO_THROW(log::Error("To standard output"));
  EXPECT_NO_THROW(log::Warning("To standard output"));
  EXPECT_NO_THROW(log::Notice("To standard output"));
  EXPECT_NO_THROW(log::Info("To standard output"));
  EXPECT_NO_THROW(log::Debug("To standard output"));
  EXPECT_NO_THROW(log::Trace("To standard output"));
}

TEST_F(LogTest, Systemlog)
{
  log::SetSysLog();
  EXPECT_NO_THROW(log::Emergency("To system log"));
  EXPECT_NO_THROW(log::Alert("To system log"));
  EXPECT_NO_THROW(log::Critical("To system log"));
  EXPECT_NO_THROW(log::Error("To system log"));
  EXPECT_NO_THROW(log::Warning("To system log"));
  EXPECT_NO_THROW(log::Notice("To system log"));
  EXPECT_NO_THROW(log::Info("To system log"));
  EXPECT_NO_THROW(log::Debug("To system log"));
  EXPECT_NO_THROW(log::Trace("To system log"));
}


LogTest::LogTest()
{
  log::SetMaxSeverity(log::SUP_SEQ_LOG_TRACE);
}

LogTest::~LogTest()
{
  log::SetMaxSeverity(log::SUP_SEQ_LOG_DEBUG);
}
