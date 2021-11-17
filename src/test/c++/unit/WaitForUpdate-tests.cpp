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
 * Copyright (c) : 2010-2021 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <SequenceParser.h>
#include <common/BasicTypes.h>
#include <gtest/gtest.h>

// Local header files

#include "LogUI.h"
#include "UnitTestHelper.h"
#include "UserInterface.h"

static const std::string WAITFORUPDATE_SUCCESS_PROCEDURE =
R"RAW(<ParallelSequence name="Parallel sequence with wait for update branch">
    <WaitForUpdate variable="target" timeout="2.0" />
    <Sequence>
        <Wait timeout="0.5"/>
        <Copy input="source" output="target"/>
    </Sequence>
</ParallelSequence>
<Workspace>
    <Local name="source"
           type='{"type":"uint64"}'
           value='1729' />
    <Local name="target"
           type='{"type":"uint64"}' />
</Workspace>
)RAW";

static const std::string WAITFORUPDATE_FAILURE_PROCEDURE =
R"RAW(<WaitForUpdate variable="target" timeout="0.2" />
<Workspace>
    <Local name="target"
           type='{"type":"uint64"}' />
</Workspace>
)RAW";

static const std::string WAITFORUPDATE_NOVAR_PROCEDURE =
R"RAW(<WaitForUpdate variable="target" timeout="0.2" />
<Workspace>
</Workspace>
)RAW";

TEST(WaitForUpdateTest, Success)
{
  const auto procedure_string = ::sup::UnitTestHelper::CreateProcedureString(WAITFORUPDATE_SUCCESS_PROCEDURE);

  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  sup::sequencer::LogUI ui;
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::SUCCESS));
}

TEST(WaitForUpdateTest, Failure)
{
  const auto procedure_string = ::sup::UnitTestHelper::CreateProcedureString(WAITFORUPDATE_FAILURE_PROCEDURE);

  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  sup::sequencer::LogUI ui;
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
}

TEST(WaitForUpdateTest, NonExistentVariable)
{
  const auto procedure_string = ::sup::UnitTestHelper::CreateProcedureString(WAITFORUPDATE_NOVAR_PROCEDURE);

  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  sup::sequencer::LogUI ui;
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
}
