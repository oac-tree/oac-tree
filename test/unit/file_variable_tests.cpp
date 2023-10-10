/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : B.Bauvir (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/generic_utils.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/variable_registry.h>

#include <sup/dto/anyvalue.h>
#include <sup/dto/json_value_parser.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

class FileVariableTest : public ::testing::Test
{
protected:
  FileVariableTest();
  virtual ~FileVariableTest();
};

TEST_F(FileVariableTest, FileWrite)
{
  const std::string body{R"(
    <Sequence>
        <Copy name="copy" inputVar="input" outputVar="file"/>
    </Sequence>
    <Workspace>
        <Local name="input"
            type='{"type":"MyStruct","attributes":[{"value":{"type":"float32"}}]}'
            value='{"value":0.0}'/>
        <File name="file"
            file="variable.bck"/>
    </Workspace>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(body);

  auto proc = ParseProcedureString(proc_str);
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  proc->ExecuteSingle(ui);
  EXPECT_EQ(proc->GetStatus(), ExecutionStatus::SUCCESS);

  EXPECT_TRUE(utils::FileExists("variable.bck"));

  sup::dto::JSONAnyValueParser parser;
  EXPECT_TRUE(parser.ParseFile("variable.bck"));
  auto value = parser.MoveAnyValue();

  EXPECT_TRUE(sup::dto::IsStructValue(value));
  EXPECT_EQ(std::string("MyStruct"), value.GetTypeName());
  EXPECT_EQ(value["value"], 0.0f);
}

TEST_F(FileVariableTest, Setup)
{
  auto variable = GlobalVariableRegistry().Create("File");

  ASSERT_NE(variable.get(), nullptr);
  EXPECT_THROW(variable->Setup(), VariableSetupException);

  EXPECT_TRUE(variable->AddAttribute("irrelevant", "undefined"));
  EXPECT_THROW(variable->Setup(), VariableSetupException);

  sup::dto::AnyValue value;
  EXPECT_FALSE(variable->GetValue(value));
  EXPECT_TRUE(sup::dto::IsEmptyValue(value));

  EXPECT_TRUE(variable->AddAttribute("file", "some_file"));
  EXPECT_NO_THROW(variable->Setup());
}

TEST_F(FileVariableTest, FileDoesNotExist)
{
  auto variable = GlobalVariableRegistry().Create("File");

  ASSERT_NE(variable.get(), nullptr);

  EXPECT_TRUE(variable->AddAttribute("file", "does_not_exist"));
  EXPECT_NO_THROW(variable->Setup());

  sup::dto::AnyValue value;  // Placeholder
  EXPECT_FALSE(variable->GetValue(value));
  EXPECT_TRUE(sup::dto::IsEmptyValue(value));
}

TEST_F(FileVariableTest, FileSuccess)
{
  const std::string body{R"(
    <Sequence>
        <!-- Expected datatype -->
        <Copy name="default" inputVar="input" outputVar="file"/>
        <Copy name="value" inputVar="value" outputVar="file.value"/>
        <!-- Should be math expression -->
        <Copy name="severity" inputVar="severity" outputVar="file.severity"/>
    </Sequence>
    <Workspace>
        <Local name="input"
            type='{"type":"MyStruct","attributes":[{"timestamp":{"type":"uint64"}},{"value":{"type":"float32"}},{"severity":{"type":"uint32"}}]}'
            value='{"timestamp":0,"value":0.0,"severity":0}'/>
        <Local name="value"
            type='{"type":"float32"}'
            value='0.1'/>
        <Local name="severity"
            type='{"type":"uint32"}'
            value='7'/>
        <File name="file" file="variable.bck"/>
    </Workspace>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(body);

  auto proc = ParseProcedureString(proc_str);

  ASSERT_TRUE(static_cast<bool>(proc));
  ASSERT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  ExecutionStatus exec = ExecutionStatus::FAILURE;

  do
  {
    proc->ExecuteSingle(ui);
    exec = proc->GetStatus();
  } while ((ExecutionStatus::SUCCESS != exec) &&
           (ExecutionStatus::FAILURE != exec));

  EXPECT_EQ(exec, ExecutionStatus::SUCCESS);
  EXPECT_TRUE(utils::FileExists("variable.bck"));

  sup::dto::JSONAnyValueParser parser;
  EXPECT_TRUE(parser.ParseFile("variable.bck"));
  auto value = parser.MoveAnyValue();

  // Test variable
  EXPECT_EQ(value["timestamp"], 0);
  EXPECT_EQ(value["value"], 0.1f);
  EXPECT_EQ(value["severity"], 7);
}


FileVariableTest::FileVariableTest() = default;

FileVariableTest::~FileVariableTest()
{
  if (utils::FileExists("variable.bck"))
  {
    std::remove("variable.bck");
  }
}
