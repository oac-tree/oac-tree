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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/oac-tree/workspace.h>

#include "unit_test_helper.h"

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

TEST(SequenceWorkspace, CopyVariable)
{
  const std::string body{R"(
    <Sequence>
        <Copy name="Copy workspace variables" inputVar="var1" outputVar="var2" />
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}' />
        <Local name="var2"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}' />
    </Workspace>
)"};

  const std::string file_name = "workspace_copyvar.xml";
  sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = ParseProcedureFile(file_name);
  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  proc->ExecuteSingle(ui);
  EXPECT_EQ(proc->GetStatus(), ExecutionStatus::SUCCESS);
}
