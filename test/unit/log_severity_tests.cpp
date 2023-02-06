/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Gennady Pospelov (IO)
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

#include <gtest/gtest.h>

#include <sup/sequencer/log_severity.h>

using namespace sup::sequencer;

class LogSeverityTest : public ::testing::Test
{
};

TEST_F(LogSeverityTest, SeverityString)
{
  using sup::sequencer::log::Severity;
  using sup::sequencer::log::SeverityString;

  EXPECT_EQ(SeverityString(Severity::SUP_SEQ_LOG_EMERG), std::string("EMERGENCY"));
  EXPECT_EQ(SeverityString(Severity::SUP_SEQ_LOG_ALERT), std::string("ALERT"));
  EXPECT_EQ(SeverityString(Severity::SUP_SEQ_LOG_CRIT), std::string("CRITICAL"));
  EXPECT_EQ(SeverityString(Severity::SUP_SEQ_LOG_ERR), std::string("ERROR"));
  EXPECT_EQ(SeverityString(Severity::SUP_SEQ_LOG_WARNING), std::string("WARNING"));
  EXPECT_EQ(SeverityString(Severity::SUP_SEQ_LOG_NOTICE), std::string("NOTICE"));
  EXPECT_EQ(SeverityString(Severity::SUP_SEQ_LOG_INFO), std::string("INFO"));
  EXPECT_EQ(SeverityString(Severity::SUP_SEQ_LOG_DEBUG), std::string("DEBUG"));
  EXPECT_EQ(SeverityString(Severity::SUP_SEQ_LOG_TRACE), std::string("TRACE"));
  EXPECT_EQ(SeverityString(Severity::NUMBER_OF_LOG_LEVELS), std::string("UNKNOWN"));
}

TEST_F(LogSeverityTest, GetSeverityFromString)
{
  using sup::sequencer::log::GetSeverityFromString;
  using sup::sequencer::log::Severity;

  EXPECT_EQ(GetSeverityFromString("EMERGENCY"), Severity::SUP_SEQ_LOG_EMERG);
  EXPECT_EQ(GetSeverityFromString("ALERT"), Severity::SUP_SEQ_LOG_ALERT);
  EXPECT_EQ(GetSeverityFromString("CRITICAL"), Severity::SUP_SEQ_LOG_CRIT);
  EXPECT_EQ(GetSeverityFromString("ERROR"), Severity::SUP_SEQ_LOG_ERR);
  EXPECT_EQ(GetSeverityFromString("WARNING"), Severity::SUP_SEQ_LOG_WARNING);
  EXPECT_EQ(GetSeverityFromString("NOTICE"), Severity::SUP_SEQ_LOG_NOTICE);
  EXPECT_EQ(GetSeverityFromString("INFO"), Severity::SUP_SEQ_LOG_INFO);
  EXPECT_EQ(GetSeverityFromString("DEBUG"), Severity::SUP_SEQ_LOG_DEBUG);
  EXPECT_EQ(GetSeverityFromString("TRACE"), Severity::SUP_SEQ_LOG_TRACE);

  EXPECT_EQ(GetSeverityFromString("UNKNOWN"), Severity::NUMBER_OF_LOG_LEVELS);
  EXPECT_EQ(GetSeverityFromString(""), Severity::NUMBER_OF_LOG_LEVELS);
}
