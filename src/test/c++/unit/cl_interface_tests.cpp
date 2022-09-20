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

#include <sup/sequencer-cli/cl_interface.h>

#include "std_redirectors.h"

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/log.h>

#include <gtest/gtest.h>

#include <cstring>
#include <memory>
#include <sstream>

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

TEST_F(CLInterfaceTest, UpdateInstructionStatus)
{
  std::ostringstream output;
  CoutRedirector redirect(output);
  EXPECT_TRUE(output.str().empty());
  cli.UpdateInstructionStatus(wait.get());
  EXPECT_TRUE(output.str().empty());
  cli_verbose.UpdateInstructionStatus(wait.get());
  EXPECT_FALSE(output.str().empty());
}

TEST_F(CLInterfaceTest, VariableUpdated)
{
  std::ostringstream output;
  CoutRedirector redirect(output);
  sup::dto::AnyValue val(sup::dto::UnsignedInteger32Type, 1234);
  std::string name = "TestUpdated";
  EXPECT_TRUE(output.str().empty());
  cli.VariableUpdated(name, val);
  EXPECT_TRUE(output.str().empty());
  cli_verbose.VariableUpdated(name, val);
  EXPECT_FALSE(output.str().empty());
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
  sup::dto::AnyValue val(sup::dto::UnsignedInteger32Type);
  std::istringstream input("567890");
  CinRedirector redirect(input);
  EXPECT_TRUE(cli.GetUserValue(val));
  auto result = val.As<sup::dto::uint32>();
  EXPECT_EQ(result, 567890u);
}

TEST_F(CLInterfaceTest, GetUserValueBool)
{
  sup::dto::AnyValue val(sup::dto::BooleanType);
  std::istringstream input("true");
  CinRedirector redirect(input);
  EXPECT_TRUE(cli.GetUserValue(val));
  auto result = val.As<sup::dto::boolean>();
  EXPECT_EQ(result, true);
}

TEST_F(CLInterfaceTest, GetUserValueString)
{
  sup::dto::AnyValue val(sup::dto::StringType);
  std::istringstream input("Test string");
  CinRedirector redirect(input);
  EXPECT_TRUE(cli.GetUserValue(val));
  auto val_str = val.As<std::string>();
  EXPECT_TRUE(val_str == "Test string");
}

TEST_F(CLInterfaceTest, GetUserValueParseError)
{
  sup::dto::AnyValue val(sup::dto::UnsignedInteger32Type);
  std::istringstream input("twenty-four");
  CinRedirector redirect(input);
  EXPECT_FALSE(cli.GetUserValue(val));
}

TEST_F(CLInterfaceTest, GetUserValueUnsupportedType)
{
  sup::dto::AnyType test_type{{
    {"timestamp", sup::dto::UnsignedInteger64Type},
    {"count", sup::dto::UnsignedInteger16Type}
  }, "CompoundTestType"};
  sup::dto::AnyValue val(test_type);
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
