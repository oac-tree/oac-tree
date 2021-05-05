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
 * Copyright (c) : 2010-2021 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

// Global header files

#include <gtest/gtest.h> // Google test framework
#include <common/log-api.h> // Syslog wrapper routines
#include <common/CompoundType.h>
#include <common/StringTools.h>
#include <memory>
#include <sstream>

// Local header files

#include "DaemonInterface.h"
#include "Instruction.h"
#include "InstructionRegistry.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

static std::ostringstream out_stream;

// Type definition

using namespace sup::sequencer;

class DaemonInterfaceTest : public ::testing::Test
{
  protected:
    DaemonInterfaceTest();
    virtual ~DaemonInterfaceTest();

    DaemonInterface daemon_interface;
    DaemonInterface daemon_interface_logging;
    std::unique_ptr<Instruction> wait;
    ccs::log::Func_t old_cb;
};

// Function declaration

static void log_cb(ccs::log::Severity_t severity, const ccs::types::char8 * const source,
                   const ccs::types::char8 * const message, va_list args);

// Global variables

// Function definition

TEST_F(DaemonInterfaceTest, UpdateInstructionStatus)
{
  EXPECT_TRUE(out_stream.str().empty());
  daemon_interface.UpdateInstructionStatus(wait.get());
  auto logged = out_stream.str();
  EXPECT_TRUE(logged.empty());
}

TEST_F(DaemonInterfaceTest, UpdateInstructionStatusLogged)
{
  EXPECT_TRUE(out_stream.str().empty());
  daemon_interface_logging.UpdateInstructionStatus(wait.get());
  auto logged = out_stream.str();
  EXPECT_FALSE(logged.empty());
  EXPECT_NE(logged.find(StatusToString(ExecutionStatus::NOT_STARTED)), std::string::npos);
}

TEST_F(DaemonInterfaceTest, StartSingleStep)
{
  EXPECT_TRUE(out_stream.str().empty());
  daemon_interface.StartSingleStep();
  EXPECT_TRUE(out_stream.str().empty());
}

TEST_F(DaemonInterfaceTest, StartSingleStepLogged)
{
  EXPECT_TRUE(out_stream.str().empty());
  daemon_interface_logging.StartSingleStep();
  EXPECT_FALSE(out_stream.str().empty());
}

TEST_F(DaemonInterfaceTest, EndSingleStep)
{
  EXPECT_TRUE(out_stream.str().empty());
  daemon_interface.EndSingleStep();
  EXPECT_TRUE(out_stream.str().empty());
}

TEST_F(DaemonInterfaceTest, EndSingleStepLogged)
{
  EXPECT_TRUE(out_stream.str().empty());
  daemon_interface_logging.EndSingleStep();
  EXPECT_FALSE(out_stream.str().empty());
}

TEST_F(DaemonInterfaceTest, GetUserValue)
{
  EXPECT_TRUE(out_stream.str().empty());
  ::ccs::types::AnyValue val(::ccs::types::UnsignedInteger32);
  val = 1234u;
  EXPECT_EQ(daemon_interface.GetUserValue(val), false);
  EXPECT_FALSE(out_stream.str().empty());
  ::ccs::types::uint32 result = val;
  EXPECT_EQ(result, 1234u);
}

TEST_F(DaemonInterfaceTest, GetUserChoice)
{
  EXPECT_TRUE(out_stream.str().empty());
  std::vector<std::string> choices = { "one", "two" };
  EXPECT_EQ(daemon_interface.GetUserChoice(choices), -1);
  EXPECT_FALSE(out_stream.str().empty());
}

DaemonInterfaceTest::DaemonInterfaceTest()
  : daemon_interface{}
  , daemon_interface_logging{true}
  , wait{GlobalInstructionRegistry().Create("Wait")}
{
  old_cb = ccs::log::SetCallback(log_cb);
  out_stream.str("");
}

DaemonInterfaceTest::~DaemonInterfaceTest()
{
  (void)ccs::log::SetCallback(old_cb);
}

static void log_cb(ccs::log::Severity_t severity, const ccs::types::char8 * const source,
                   const ccs::types::char8 * const message, va_list args)
{
  ccs::types::char8 buffer [1024];
  ccs::types::char8* p_buf = static_cast<ccs::types::char8*>(buffer);
  size_t size = 1024u;

  (void)vsnprintf(p_buf, size, message, args);

  // Re-align pointer
  size -= strlen(p_buf);
  p_buf += strlen(p_buf);

  (void)ccs::HelperTools::SafeStringCopy(p_buf, "\n", size);

  // Ensure proper termination
  buffer[1023] = '\0';

  out_stream << buffer;
}

#undef LOG_ALTERN_SRC
