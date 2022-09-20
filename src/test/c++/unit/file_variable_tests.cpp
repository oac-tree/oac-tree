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

#include "log_ui.h"
#include "unit_test_helper.h"

#include <sup/sequencer/generic_utils.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/variable_registry.h>

#include <gtest/gtest.h>

class FileVariableTest : public ::testing::Test
{
protected:
  FileVariableTest();
  virtual ~FileVariableTest();
};

TEST_F(FileVariableTest, File_write)
{
  const std::string body{R"(
    <Sequence>
        <Copy name="copy"
            input="input"
            output="file"/>
    </Sequence>
    <Workspace>
        <Local name="input"
            type='{"type":"MyStruct","attributes":[{"value":{"type":"float32"}}]}'
            value='{"value":0.0}'/>
        <FileVariable name="file"
            file="variable.bck"/>
    </Workspace>
)"};

  const std::string file_name = "variable_file.xml";
  ::sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, ::sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = sup::sequencer::ParseProcedureFile(file_name);
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_TRUE(proc->Setup());

  sup::sequencer::LogUI ui;
  proc->ExecuteSingle(&ui);
  EXPECT_EQ(proc->GetStatus(), sup::sequencer::ExecutionStatus::SUCCESS);

  EXPECT_TRUE(sup::sequencer::utils::FileExists("variable.bck"));

  sup::dto::AnyValue value = sup::dto::AnyValueFromJSONFile("variable.bck");

  EXPECT_TRUE(sup::dto::IsStructValue(value));
  EXPECT_EQ(std::string("MyStruct"), value.GetTypeName());
  EXPECT_EQ(value["value"], 0.0f);
}

TEST_F(FileVariableTest, Setup_error)
{
  auto variable = sup::sequencer::GlobalVariableRegistry().Create("FileVariable");

  ASSERT_NE(variable.get(), nullptr);

  EXPECT_TRUE(variable->AddAttribute("irrelevant", "undefined"));
  variable->Setup();

  sup::dto::AnyValue value;  // Placeholder
  EXPECT_FALSE(variable->GetValue(value));
  EXPECT_TRUE(sup::dto::IsEmptyValue(value));
}

TEST_F(FileVariableTest, File_error)
{
  auto variable = sup::sequencer::GlobalVariableRegistry().Create("FileVariable");

  ASSERT_NE(variable.get(), nullptr);

  EXPECT_TRUE(variable->AddAttribute("file", "undefined"));
  variable->Setup();

  sup::dto::AnyValue value;  // Placeholder
  EXPECT_FALSE(variable->GetValue(value));
  EXPECT_TRUE(sup::dto::IsEmptyValue(value));
}

TEST_F(FileVariableTest, File_attr)
{
  const std::string body{R"(
    <Sequence>
        <!-- Expected datatype -->
        <Copy name="default"
            input="input"
            output="file"/>
        <Copy name="value"
            input="value"
            output="file.value"/>
        <!-- Should be math expression -->
        <Copy name="severity"
            input="severity"
            output="file.severity"/>
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
        <FileVariable name="file"
            file="variable.bck"/>
    </Workspace>
)"};

  const std::string file_name = "variable_attr.xml";
  ::sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, ::sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = sup::sequencer::ParseProcedureFile(file_name);

  ASSERT_NE(proc.get(), nullptr);
  ASSERT_TRUE(proc->Setup());

  sup::sequencer::LogUI ui;
  sup::sequencer::ExecutionStatus exec = sup::sequencer::ExecutionStatus::FAILURE;

  do
  {
    proc->ExecuteSingle(&ui);
    exec = proc->GetStatus();
  } while ((sup::sequencer::ExecutionStatus::SUCCESS != exec)
           && (sup::sequencer::ExecutionStatus::FAILURE != exec));

  EXPECT_EQ(exec, sup::sequencer::ExecutionStatus::SUCCESS);
  EXPECT_TRUE(sup::sequencer::utils::FileExists("variable.bck"));

  sup::dto::AnyValue value = sup::dto::AnyValueFromJSONFile("variable.bck");

  // Test variable
  EXPECT_EQ(value["timestamp"], 0);
  EXPECT_EQ(value["value"], 0.1f);
  EXPECT_EQ(value["severity"], 7);
}


FileVariableTest::FileVariableTest() = default;

FileVariableTest::~FileVariableTest()
{
  if (sup::sequencer::utils::FileExists("variable.bck"))
  {
    std::remove("variable.bck");
  }
}
