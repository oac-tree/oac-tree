/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Carlos Miguel Silva (EXT)
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

#include <gtest/gtest.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

using namespace sup::sequencer;

TEST(AddMember, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("AddMember");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("inputVar", "input_var"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
  EXPECT_TRUE(instr->AddAttribute("outputVar", "output_var"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
  EXPECT_TRUE(instr->AddAttribute("varName", "var_name"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(AddMember, AddedUint8Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint8"}'
               value='125'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedInt8Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"int8"}'
               value='-100'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedUint16Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint16"}'
               value='125'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedInt16Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"int8"}'
               value='-100'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedUint32Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint32"}'
               value='12345'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedInt32Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"int32"}'
               value='-1234'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedUint64Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint64"}'
               value='12345'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedInt64Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"int64"}'
               value='12345'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedFloat32Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"float32"}'
               value='12.3'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedFloat64Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"float64"}'
               value='12.3'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedChar8Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"char8"}'
               value='12'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedBoolSuccessfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"bool"}'
               value='0'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedUint32ToArraySuccessfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint32"}'
               value='12345'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","attributes":[{"val1": {"type":"uint32"}}]}' 
               value='{"val1":1729}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedStructToArrayOfStructsSuccessfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2.val1"/>
        <Equals leftVar="var1" rightVar="var2.val1.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type": "uint64_struct","attributes": [{"pars": {"type": "uint64"}}]}'
               value='{"pars":12345}'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","attributes":[{"val1": {"type": "uint64_struct","attributes": [{"val11": {"type": "uint64"}}, {"val12": {"type": "uint64"}}]}}]}'
               value='{"val1.val11":1729, "val1.val12":1728}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedToMultiLevelStructSuccessfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2.val2"/>
        <Equals leftVar="var1" rightVar="var2.val2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"bool"}'
               value='12345'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type": "uint64_struct","attributes": [{"val1": {"type": "uint64"}},{"val2": {"type": "uint64_struct","attributes": [{"val22": {"type": "uint64"}}]}}]}'
               value='{"val1":1729, "val2.val22":1728}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, AddedFromAndToMultiLevelStructSuccessfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1.pars" varName="a" outputVar="var2.system.params"/>
        <Equals leftVar="var1.pars" rightVar="var2.system.params.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type": "uint64_struct","attributes": [{"pars": {"type": "uint64"}},{"val2": {"type": "uint64_struct","attributes": [{"val22": {"type": "uint64"}}]}}]}'
               value='{"pars":12345, "val2.val22":123}'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type": "uint64_struct","attributes": [{"val1": {"type": "uint64"}},{"system": {"type": "uint64_struct","attributes": [{"params": {"type": "uint64_struct","attributes": [{"val22": {"type": "uint64"}}]}}]}}]}' 
               value='{"val1":1729, "system.params.val22":1728}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, FailIncorrectXmlSequence)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint32"}'
               value='12345'/>
        <Local name="var2"
               type='{"type":"uint64_struct","attributes":[{"a":{"type":"uint64"}}]}'
               value='{"a":12345}'/>'
               dynamicType="true" />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, FailMissingVariable)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var" varName="a" outputVar="var2"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint32"}'
               value='12345'/>
        <Local name="var2"
               dynamicType="true" 
               type='{"type":"uint64_struct","attributes":[{"a":{"type":"uint64"}}]}'
               value='{"a":12345}'/>' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, FailEmptyVarName)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var" varName="" outputVar="var2"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint32"}'
               value='12345'/>
        <Local name="var2"
               dynamicType="true" 
               type='{"type":"uint64_struct","attributes":[{"a":{"type":"uint64"}}]}'
               value='{"a":12345}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, FailInvalidVarName)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var" varName="invalid.name" outputVar="var2"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint32"}'
               value='12345'/>
        <Local name="var2"
               dynamicType="true" 
               type='{"type":"uint64_struct","attributes":[{"a":{"type":"uint64"}}]}'
               value='{"a":12345}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, FailMemberAlreadyExists)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint32"}'
               value='12345'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"uint64_struct","attributes":[{"a":{"type":"uint64"}}]}'
               value='{"a":12345}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, FailTypeLocked)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint32"}'
               value='12345'/>
        <Local name="var2"
               dynamicType="false"
               type='{"type":"uint64_struct","attributes":[{"b":{"type":"uint64"}}]}'
               value='{"b":12345}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, FailTypeLockedAddToArrayDifferentTypes)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint32"}'
               value='12345'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","attributes":[{"val1": {"type": "uint64_struct","attributes": [{"val11": {"type": "uint64"}}, {"val12": {"type": "uint64"}}]}},{"val2": {"type": "uint64_struct","attributes": [{"val21": {"type": "uint64"}}, {"val22": {"type": "uint64"}}]}}]}'
               value='{"val1.val11":1729, "val1.val12":1728, "val2.val21":1727, "val2.val22":1726}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}