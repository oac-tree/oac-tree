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
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/sequencer/instruction_registry.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::sequencer;

class TestInstruction : public Instruction
{
public:
  TestInstruction() : Instruction(TestInstruction::Type) {}
  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface&, Workspace&) override
  {
    return ExecutionStatus::SUCCESS;
  }
};

const std::string TestInstruction::Type = "TestInstruction";

TEST(InstructionRegistry, RegisterInstruction)
{
  // no such name
  auto registered_names = GlobalInstructionRegistry().RegisteredInstructionNames();
  auto it = std::find(registered_names.begin(), registered_names.end(), TestInstruction::Type);
  EXPECT_TRUE(it == registered_names.end());

  // not possible to create instruction
  EXPECT_THROW(GlobalInstructionRegistry().Create(TestInstruction::Type),
               InvalidOperationException);

  // registration
  EXPECT_TRUE(RegisterGlobalInstruction<TestInstruction>());

  // second registration with the same name is not allowed
  EXPECT_THROW(RegisterGlobalInstruction<TestInstruction>(), InvalidOperationException);

  // there is a name
  registered_names = GlobalInstructionRegistry().RegisteredInstructionNames();
  it = std::find(registered_names.begin(), registered_names.end(), TestInstruction::Type);
  EXPECT_FALSE(it == registered_names.end());

  // can create instruction
  auto inst = GlobalInstructionRegistry().Create(TestInstruction::Type);
  EXPECT_TRUE(static_cast<bool>(inst));
}

TEST(InstructionRegistry, CreateFailure)
{
  EXPECT_THROW(GlobalInstructionRegistry().Create("UndefinedInstructionName"),
               InvalidOperationException);
}
