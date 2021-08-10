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
 * Copyright (c) : 2010-2020 ITER Organization,
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

#include <common/CompoundType.h>
#include <common/StringTools.h>
#include <gtest/gtest.h>  // Google test framework

#include <memory>
#include <sstream>

#include <common/log-api.h>  // Syslog wrapper routines

// Local header files

#include "CLInterface.h"
#include "Instruction.h"
#include "InstructionRegistry.h"
#include "StdRedirectors.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

using namespace sup::sequencer;

class CLInterfaceTest : public ::testing::Test
{
protected:
  CLInterfaceTest();
  virtual ~CLInterfaceTest();

  CLInterface cli;
  CLInterface cli_verbose;
  std::unique_ptr<Instruction> wait;
};

// Function declaration

// Global variables

static ::ccs::log::Func_t __handler = ::ccs::log::SetStdout();

// Function definition

TEST_F(CLInterfaceTest, UpdateInstructionStatus)
{
  std::ostringstream output;
  CoutRedirector redirect(output);
  EXPECT_TRUE(output.str().empty());
  cli.UpdateInstructionStatus(wait.get());
  EXPECT_TRUE(output.str().empty());
}

TEST_F(CLInterfaceTest, StartSingleStep)
{
  std::ostringstream output;
  CoutRedirector redirect(output);
  EXPECT_TRUE(output.str().empty());
  cli_verbose.StartSingleStep();
  EXPECT_FALSE(output.str().empty());
}

TEST_F(CLInterfaceTest, EndSingleStep)
{
  std::ostringstream output;
  CoutRedirector redirect(output);
  EXPECT_TRUE(output.str().empty());
  cli_verbose.EndSingleStep();
  EXPECT_FALSE(output.str().empty());
}

TEST_F(CLInterfaceTest, GetUserValue)
{
  ::ccs::types::AnyValue val(::ccs::types::UnsignedInteger32);
  std::istringstream input("567890");
  CinRedirector redirect(input);
  EXPECT_TRUE(cli.GetUserValue(val));
  ::ccs::types::uint32 result = val;
  EXPECT_EQ(result, 567890u);
}

TEST_F(CLInterfaceTest, GetUserValueBool)
{
  ::ccs::types::AnyValue val(::ccs::types::Boolean);
  std::istringstream input("true");
  CinRedirector redirect(input);
  EXPECT_TRUE(cli.GetUserValue(val));
  bool result = val;
  EXPECT_EQ(result, true);
}

TEST_F(CLInterfaceTest, GetUserValueString)
{
  ::ccs::types::AnyValue val(::ccs::types::String);
  std::istringstream input("Test string");
  CinRedirector redirect(input);
  EXPECT_TRUE(cli.GetUserValue(val));
  char buffer[STRING_MAX_LENGTH];
  EXPECT_TRUE(val.SerialiseInstance(buffer, STRING_MAX_LENGTH));
  EXPECT_EQ(::ccs::HelperTools::StringCompare("Test string", buffer), 0);
}

TEST_F(CLInterfaceTest, GetUserValueParseError)
{
  ::ccs::types::AnyValue val(::ccs::types::UnsignedInteger32);
  std::istringstream input("twenty-four");
  CinRedirector redirect(input);
  EXPECT_FALSE(cli.GetUserValue(val));
}

TEST_F(CLInterfaceTest, GetUserValueUnsupportedType)
{
  auto test_type = (new ::ccs::types::CompoundType("CompoundTestType"))
                       ->AddAttribute("timestamp", "uint64")
                       ->AddAttribute("count", "uint16");
  ::ccs::types::AnyValue val(test_type);
  std::istringstream input("17");
  CinRedirector redirect(input);
  EXPECT_FALSE(cli.GetUserValue(val));
}

TEST_F(CLInterfaceTest, GetUserChoice)
{
  std::vector<std::string> choices = {"one", "two"};
  std::istringstream input("1");
  CinRedirector redirect(input);
  auto choice = cli.GetUserChoice(choices);
  EXPECT_EQ(choice, 1);
}

TEST_F(CLInterfaceTest, GetUserChoiceParseError)
{
  std::vector<std::string> choices = {"one", "two"};
  std::istringstream input("one");
  CinRedirector redirect(input);
  auto choice = cli.GetUserChoice(choices);
  EXPECT_EQ(choice, -1);
}

TEST_F(CLInterfaceTest, GetUserChoiceOutOfBounds)
{
  std::vector<std::string> choices = {"one", "two"};
  std::istringstream input("2");
  CinRedirector redirect(input);
  auto choice = cli.GetUserChoice(choices);
  EXPECT_EQ(choice, -1);
}

CLInterfaceTest::CLInterfaceTest()
    : cli{}, cli_verbose{true}, wait{GlobalInstructionRegistry().Create("Wait")}
{
}

CLInterfaceTest::~CLInterfaceTest() {}

#undef LOG_ALTERN_SRC
