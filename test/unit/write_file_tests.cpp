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
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "unit_test_helper.h"

#include <sup/oac-tree/generic_utils.h>
#include <sup/oac-tree/sequence_parser.h>

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/json_value_parser.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

class WriteFileTest : public ::testing::Test
{
protected:
  WriteFileTest() = default;
  ~WriteFileTest() override;
};

WriteFileTest::~WriteFileTest()
{
  if (utils::FileExists("write_file_test.json"))
  {
    std::remove("write_file_test.json");
  }
}

TEST_F(WriteFileTest, Success)
{
  const std::string body{R"(
    <Sequence>
        <WriteFile inputVar="input" file="write_file_test.json"/>
    </Sequence>
    <Workspace>
        <Local name="input"
            type='{"type":"","attributes":[{"value":{"type":"float32"}}]}'
            value='{"value":0.1}'/>
    </Workspace>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(body);

  auto proc = ParseProcedureString(proc_str);
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));

  EXPECT_TRUE(utils::FileExists("write_file_test.json"));

  sup::dto::JSONAnyValueParser parser;
  EXPECT_TRUE(parser.ParseFile("write_file_test.json"));
  auto value = parser.MoveAnyValue();
  EXPECT_EQ(value["value"], 0.1f);
}

TEST_F(WriteFileTest, ScalarValue)
{
  const std::string body{R"(
    <Sequence>
        <WriteFile inputVar="input" file="write_file_test.json"/>
    </Sequence>
    <Workspace>
        <Local name="input" type='{"type":"float64"}' value='3.14'/>
    </Workspace>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(body);

  auto proc = ParseProcedureString(proc_str);
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));

  EXPECT_TRUE(utils::FileExists("write_file_test.json"));

  sup::dto::JSONAnyValueParser parser;
  EXPECT_TRUE(parser.ParseFile("write_file_test.json"));
  auto value = parser.MoveAnyValue();
  EXPECT_EQ(value, sup::dto::AnyValue(sup::dto::Float64Type, 3.14));
}

TEST_F(WriteFileTest, VariableAttribute)
{
  const std::string body{R"(
    <Sequence>
        <WriteFile inputVar="input" file="@filename"/>
    </Sequence>
    <Workspace>
        <Local name="filename" type='{"type":"string"}' value='"write_file_test.json"'/>
        <Local name="input" type='{"type":"float64"}' value='3.14'/>
    </Workspace>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(body);

  auto proc = ParseProcedureString(proc_str);
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));

  EXPECT_TRUE(utils::FileExists("write_file_test.json"));

  sup::dto::JSONAnyValueParser parser;
  EXPECT_TRUE(parser.ParseFile("write_file_test.json"));
  auto value = parser.MoveAnyValue();
  EXPECT_EQ(value, sup::dto::AnyValue(sup::dto::Float64Type, 3.14));
}

TEST_F(WriteFileTest, MissingInputVariable)
{
  const std::string body{R"(
    <Sequence>
        <WriteFile inputVar="nonexistent" file="write_file_test.json"/>
    </Sequence>
    <Workspace>
    </Workspace>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(body);

  auto proc = ParseProcedureString(proc_str);
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::TestLogUserInterface ui;
  EXPECT_TRUE(
    sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(WriteFileTest, StructValue)
{
  const std::string body{R"(
    <Sequence>
        <WriteFile inputVar="input" file="write_file_test.json"/>
    </Sequence>
    <Workspace>
        <Local name="input"
            type='{"type":"","attributes":[{"timestamp":{"type":"uint64"}},{"value":{"type":"float32"}},{"severity":{"type":"uint32"}}]}'
            value='{"timestamp":12345,"value":0.5,"severity":3}'/>
    </Workspace>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(body);

  auto proc = ParseProcedureString(proc_str);
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));

  EXPECT_TRUE(utils::FileExists("write_file_test.json"));

  sup::dto::JSONAnyValueParser parser;
  EXPECT_TRUE(parser.ParseFile("write_file_test.json"));
  auto value = parser.MoveAnyValue();
  EXPECT_EQ(value["timestamp"], static_cast<sup::dto::uint64>(12345));
  EXPECT_EQ(value["value"], 0.5f);
  EXPECT_EQ(value["severity"], static_cast<sup::dto::uint32>(3));
}

TEST_F(WriteFileTest, WriteAndReadBack)
{
  const std::string body{R"(
    <Sequence>
        <WriteFile inputVar="input" file="write_file_test.json"/>
        <ReadFile file="write_file_test.json" outputVar="output"/>
        <Equals leftVar="input" rightVar="output"/>
    </Sequence>
    <Workspace>
        <Local name="input"
            type='{"type":"","attributes":[{"name":{"type":"string"}},{"count":{"type":"uint32"}}]}'
            value='{"name":"test","count":7}'/>
        <Local name="output"
            type='{"type":"","attributes":[{"name":{"type":"string"}},{"count":{"type":"uint32"}}]}'
            value='{"name":"","count":0}'/>
    </Workspace>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(body);

  auto proc = ParseProcedureString(proc_str);
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(WriteFileTest, PrettyJson)
{
  const std::string body{R"(
    <Sequence>
        <WriteFile inputVar="input" file="write_file_test.json" pretty="true"/>
    </Sequence>
    <Workspace>
        <Local name="input"
            type='{"type":"","attributes":[{"name":{"type":"string"}},{"count":{"type":"uint32"}}]}'
            value='{"name":"hello","count":5}'/>
    </Workspace>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(body);

  auto proc = ParseProcedureString(proc_str);
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));

  EXPECT_TRUE(utils::FileExists("write_file_test.json"));

  // Verify the file contains newlines (pretty printed)
  auto content = sup::UnitTestHelper::GetTextFileContent("write_file_test.json");
  EXPECT_EQ(content.find("[\n"), 0);

  // Verify the content is still valid and parseable
  sup::dto::JSONAnyValueParser parser;
  EXPECT_TRUE(parser.ParseFile("write_file_test.json"));
  auto value = parser.MoveAnyValue();
  EXPECT_EQ(value["name"], sup::dto::AnyValue(sup::dto::StringType, "hello"));
  EXPECT_EQ(value["count"], static_cast<sup::dto::uint32>(5));
}
TEST_F(WriteFileTest, WriteAndReadBackWithFileVariable)
{
  const std::string body{R"(
    <Sequence>
        <WriteFile inputVar="input" file="write_file_test.json"/>
        <Equals leftVar="file_var" rightVar="input"/>
    </Sequence>
    <Workspace>
        <Local name="input"
            type='{"type":"","attributes":[{"value":{"type":"float32"}},{"severity":{"type":"uint32"}}]}'
            value='{"value":2.5,"severity":4}'/>
        <File name="file_var" file="write_file_test.json"/>
    </Workspace>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(body);

  auto proc = ParseProcedureString(proc_str);
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}