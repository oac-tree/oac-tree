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
// // <AddMember inputVar="var1.pars" varName="a" outputVar="var2.systems.params"/>
TEST(AddMember, AddedSuccessFully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="a" varName="a" outputVar="var2"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='1' />
        <Local name="var1"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}' />
        <Local name="var2"
               type='{"type":"uint64_struct","attributes":[{"a":{"type":"uint64"}}]}' />
               a='{"value":1729}' />
        <Local name="dynamic_var"
               type='{"type":"uint32"}'
               value='0'
               dynamicType="true"/>
        <Local name="static_var"
               type='{"type":"uint32"}'
               value='0'
               dynamicType="false"/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, FailAlreadyExists)
{
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="a" varName="a" outputVar="var2"/>
    </Sequence>
    <Workspace>
        <Local name="var1" type='{"type":"uint8"}' value='1' />
        <Local name="var2"
               type='{"type":"uint64_struct","attributes":[{"a":{"type":"uint64"}}]}' />
               a='{"value":1729}' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddMember, DISABLED_FailTypeLocked)
{
    //How to to define a type locked variable?
  const std::string body{
      R"(
    <Sequence>
        <AddMember inputVar="a" varName="a" outputVar="var2"/>
    </Sequence>
    <Workspace>
        <Local name="var1" type='{"type":"uint8"}' value='1' />
        <Local name="var2"
               type='{"type":"uint64_struct","attributes":[{"a":{"type":"uint64"}}]}' />
               a='{"value":1729}' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}