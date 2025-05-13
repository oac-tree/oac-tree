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
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

const std::string AsyncDecoratorString =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/oac-tree" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/oac-tree oac-tree.xsd">
    <ParallelSequence>
        <ReactiveFallback>
            <Equals leftVar="break" rightVar="one"/>
            <Async>
                <Wait timeout="10.0"/>
            </Async>
        </ReactiveFallback>
        <Sequence>
            <Wait timeout="0.2" />
            <Copy inputVar="one" outputVar="break"/>
        </Sequence>
    </ParallelSequence>
    <Workspace>
        <Local name="break" type='{"type":"uint32"}' value="0"/>
        <Local name="one" type='{"type":"uint32"}' value="1"/>
    </Workspace>
</Procedure>
)RAW";

TEST(AsyncDecorator, ReactiveProcedure)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(AsyncDecoratorString);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AsyncDecorator, Setup)
{
  auto async_dec = GlobalInstructionRegistry().Create("Async");
  ASSERT_NE(async_dec.get(), nullptr);
  EXPECT_TRUE(async_dec->ChildInstructions().empty());

  auto child = GlobalInstructionRegistry().Create("Wait");
  EXPECT_TRUE(child->AddAttribute("timeout", "1.0"));
  EXPECT_TRUE(AppendChildInstruction(*async_dec, std::move(child)));
  EXPECT_FALSE(async_dec->ChildInstructions().empty());

  Procedure proc;
  EXPECT_NO_THROW(async_dec->Setup(proc));
}
