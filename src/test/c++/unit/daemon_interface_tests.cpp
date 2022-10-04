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

#include <sup/sequencer-daemon/daemon_interface.h>

#include <sup/sequencer/log.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>

#include <gtest/gtest.h>

#include <memory>
#include <sstream>

using namespace sup::sequencer;

class DaemonInterfaceTest : public ::testing::Test
{
protected:
  DaemonInterfaceTest();
  virtual ~DaemonInterfaceTest();

  DaemonInterface daemon_interface;
  DaemonInterface daemon_interface_logging;
  std::unique_ptr<Instruction> wait;
  std::ostringstream out_stream;
};

TEST_F(DaemonInterfaceTest, UpdateInstructionStatus)
{
  log::LogStreamRedirector redirector(out_stream);
  EXPECT_TRUE(out_stream.str().empty());
  daemon_interface.UpdateInstructionStatus(wait.get());
  auto logged = out_stream.str();
  EXPECT_TRUE(logged.empty());
}

TEST_F(DaemonInterfaceTest, UpdateInstructionStatusLogged)
{
  log::LogStreamRedirector redirector(out_stream);
  EXPECT_TRUE(out_stream.str().empty());
  daemon_interface_logging.UpdateInstructionStatus(wait.get());
  auto logged = out_stream.str();
  EXPECT_FALSE(logged.empty());
  EXPECT_NE(logged.find(StatusToString(ExecutionStatus::NOT_STARTED)), std::string::npos);
}

TEST_F(DaemonInterfaceTest, StartSingleStep)
{
  log::LogStreamRedirector redirector(out_stream);
  EXPECT_TRUE(out_stream.str().empty());
  daemon_interface.StartSingleStep();
  EXPECT_TRUE(out_stream.str().empty());
}

TEST_F(DaemonInterfaceTest, StartSingleStepLogged)
{
  log::LogStreamRedirector redirector(out_stream);
  EXPECT_TRUE(out_stream.str().empty());
  daemon_interface_logging.StartSingleStep();
  EXPECT_FALSE(out_stream.str().empty());
}

TEST_F(DaemonInterfaceTest, EndSingleStep)
{
  log::LogStreamRedirector redirector(out_stream);
  EXPECT_TRUE(out_stream.str().empty());
  daemon_interface.EndSingleStep();
  EXPECT_TRUE(out_stream.str().empty());
}

TEST_F(DaemonInterfaceTest, EndSingleStepLogged)
{
  log::LogStreamRedirector redirector(out_stream);
  EXPECT_TRUE(out_stream.str().empty());
  daemon_interface_logging.EndSingleStep();
  EXPECT_FALSE(out_stream.str().empty());
}

TEST_F(DaemonInterfaceTest, GetUserValue)
{
  log::LogStreamRedirector redirector(out_stream);
  EXPECT_TRUE(out_stream.str().empty());
  sup::dto::AnyValue val(sup::dto::UnsignedInteger32Type, 1234);
  EXPECT_EQ(daemon_interface.GetUserValue(val), false);
  EXPECT_FALSE(out_stream.str().empty());
  auto result = val.As<sup::dto::uint32>();
  EXPECT_EQ(result, 1234u);
}

TEST_F(DaemonInterfaceTest, GetUserChoice)
{
  log::LogStreamRedirector redirector(out_stream);
  EXPECT_TRUE(out_stream.str().empty());
  std::vector<std::string> choices = {"one", "two"};
  EXPECT_EQ(daemon_interface.GetUserChoice(choices), -1);
  EXPECT_FALSE(out_stream.str().empty());
}

TEST_F(DaemonInterfaceTest, PutValue)
{
  log::LogStreamRedirector redirector(out_stream);
  sup::dto::AnyValue val = 23;
  EXPECT_TRUE(daemon_interface_logging.PutValue(val));
  EXPECT_NE(out_stream.str().find("23"), std::string::npos);
}

TEST_F(DaemonInterfaceTest, Message)
{
  log::LogStreamRedirector redirector(out_stream);
  std::string message = "Hello message";
  EXPECT_NO_THROW(daemon_interface_logging.Message(message));
  EXPECT_NE(out_stream.str().find(message), std::string::npos);
}

DaemonInterfaceTest::DaemonInterfaceTest()
    : daemon_interface{}
    , daemon_interface_logging{true}
    , wait{GlobalInstructionRegistry().Create("Wait")}
    , out_stream{}
{}

DaemonInterfaceTest::~DaemonInterfaceTest() = default;
