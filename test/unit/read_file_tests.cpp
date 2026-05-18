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

#include <gtest/gtest.h>

using namespace sup::oac_tree;

class ReadFileTest : public ::testing::Test
{
protected:
  ReadFileTest() = default;
  ~ReadFileTest() override;
};

ReadFileTest::~ReadFileTest()
{
  if (utils::FileExists("read_file_test.json"))
  {
    std::remove("read_file_test.json");
  }
}

TEST_F(ReadFileTest, Success)
{
  sup::dto::AnyValue file_value{{"value", {sup::dto::Float32Type, 0.1f}}};
  const std::string json_content = sup::dto::AnyValueToJSONString(file_value);

  sup::UnitTestHelper::TemporaryTestFile json_file("read_file_test.json", json_content);

  const std::string body{R"(
    <Sequence>
        <ReadFile file="read_file_test.json" outputVar="output"/>
        <Equals leftVar="output" rightVar="expected"/>
    </Sequence>
    <Workspace>
        <Local name="output"
            type='{"type":"","attributes":[{"value":{"type":"float32"}}]}'
            value='{"value":0.0}'/>
        <Local name="expected"
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
}

TEST_F(ReadFileTest, ScalarValue)
{
  sup::dto::AnyValue file_value{sup::dto::Float64Type, 3.14};
  const std::string json_content = sup::dto::AnyValueToJSONString(file_value);

  sup::UnitTestHelper::TemporaryTestFile json_file("read_file_test.json", json_content);

  const std::string body{R"(
    <Sequence>
        <ReadFile file="read_file_test.json" outputVar="output"/>
        <Equals leftVar="output" rightVar="expected"/>
    </Sequence>
    <Workspace>
        <Local name="output" type='{"type":"float64"}' value='0.0'/>
        <Local name="expected" type='{"type":"float64"}' value='3.14'/>
    </Workspace>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(body);

  auto proc = ParseProcedureString(proc_str);
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(ReadFileTest, VariableAttribute)
{
  sup::dto::AnyValue file_value{sup::dto::Float64Type, 3.14};
  const std::string json_content = sup::dto::AnyValueToJSONString(file_value);

  sup::UnitTestHelper::TemporaryTestFile json_file("read_file_test.json", json_content);

  const std::string body{R"(
    <Sequence>
        <ReadFile file="@filename" outputVar="output"/>
        <Equals leftVar="output" rightVar="expected"/>
    </Sequence>
    <Workspace>
        <Local name="filename" type='{"type":"string"}' value='"read_file_test.json"'/>
        <Local name="output" type='{"type":"float64"}' value='0.0'/>
        <Local name="expected" type='{"type":"float64"}' value='3.14'/>
    </Workspace>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(body);

  auto proc = ParseProcedureString(proc_str);
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(ReadFileTest, FileDoesNotExist)
{
  const std::string body{R"(
    <Sequence>
        <ReadFile file="does_not_exist.json" outputVar="output"/>
    </Sequence>
    <Workspace>
        <Local name="output" type='{"type":"uint32"}' value='0'/>
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

TEST_F(ReadFileTest, InvalidJson)
{
  const std::string json_content = "{ not valid json }";

  sup::UnitTestHelper::TemporaryTestFile json_file("read_file_test.json", json_content);

  const std::string body{R"(
    <Sequence>
        <ReadFile file="read_file_test.json" outputVar="output"/>
    </Sequence>
    <Workspace>
        <Local name="output" type='{"type":"uint32"}' value='0'/>
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

TEST_F(ReadFileTest, MissingOutputVariable)
{
  sup::dto::AnyValue file_value{sup::dto::UnsignedInteger32Type, 42};
  const std::string json_content = sup::dto::AnyValueToJSONString(file_value);

  sup::UnitTestHelper::TemporaryTestFile json_file("read_file_test.json", json_content);

  const std::string body{R"(
    <Sequence>
        <ReadFile file="read_file_test.json" outputVar="nonexistent"/>
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

TEST_F(ReadFileTest, StructValue)
{
  sup::dto::AnyValue file_value{{"timestamp", {sup::dto::UnsignedInteger64Type, 12345}},
                                {"value", {sup::dto::Float32Type, 0.5f}},
                                {"severity", {sup::dto::UnsignedInteger32Type, 3}}};
  const std::string json_content = sup::dto::AnyValueToJSONString(file_value);

  sup::UnitTestHelper::TemporaryTestFile json_file("read_file_test.json", json_content);

  const std::string body{R"(
    <Sequence>
        <ReadFile file="read_file_test.json" outputVar="output"/>
        <Equals leftVar="output" rightVar="expected"/>
    </Sequence>
    <Workspace>
        <Local name="output"
            type='{"type":"","attributes":[{"timestamp":{"type":"uint64"}},{"value":{"type":"float32"}},{"severity":{"type":"uint32"}}]}'
            value='{"timestamp":0,"value":0.0,"severity":0}'/>
        <Local name="expected"
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
}
