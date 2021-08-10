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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "LogUI.h"
#include "SequenceParser.h"
#include "UnitTestHelper.h"
#include "Variable.h"
#include "VariableRegistry.h"

#include <common/AnyValueHelper.h>
#include <gtest/gtest.h>

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

static inline bool Terminate(void)
{
  bool status = false;
  if (ccs::HelperTools::Exist("/tmp/variable.bck"))
  {
    status = (std::remove("/tmp/variable.bck") == 0);
  }
  return status;
}

TEST(FileVariable, File_write)
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
            file="/tmp/variable.bck"/>
    </Workspace>
)"};

  const std::string file_name = "/tmp/variable_file.xml";
  ::sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, ::sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = sup::sequencer::ParseProcedureFile(file_name);
  ASSERT_TRUE(proc.get() != nullptr);

  sup::sequencer::LogUI ui;
  proc->ExecuteSingle(&ui);
  EXPECT_EQ(proc->GetStatus(), sup::sequencer::ExecutionStatus::SUCCESS);

  EXPECT_TRUE(ccs::HelperTools::Exist("/tmp/variable.bck"));

  ccs::types::AnyValue value;  // Placeholder

  EXPECT_TRUE(ccs::HelperTools::ReadFromFile(&value, "/tmp/variable.bck"));

  ASSERT_TRUE(static_cast<bool>(value.GetType()));
  // TODO check variable

  Terminate();
}

TEST(FileVariable, Setup_error)
{
  auto variable = sup::sequencer::GlobalVariableRegistry().Create("FileVariable");

  bool status = static_cast<bool>(variable);

  if (status)
  {  // Missing mandatory attribute .. Setup implicit
    // status = (false == variable->Setup());
    status = variable->AddAttribute("irrelevant", "undefined");
  }

  ccs::types::AnyValue value;  // Placeholder

  if (status)
  {
    status =
        ((false == variable->GetValue(value)) && (false == static_cast<bool>(value.GetType())));
  }

  ASSERT_EQ(true, status);
}

TEST(FileVariable, File_error)
{
  auto variable = sup::sequencer::GlobalVariableRegistry().Create("FileVariable");

  bool status = static_cast<bool>(variable);

  if (status)
  {
    status = variable->AddAttribute("file", "undefined");
  }

  ccs::types::AnyValue value;  // Placeholder

  if (status)
  {
    status =
        ((false == variable->GetValue(value)) && (false == static_cast<bool>(value.GetType())));
  }

  ASSERT_EQ(true, status);
}

TEST(FileVariable, File_attr)
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
            file="/tmp/variable.bck"/>
    </Workspace>
)"};

  const std::string file_name = "/tmp/variable_attr.xml";
  ::sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, ::sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = sup::sequencer::ParseProcedureFile(file_name);

  bool status = bool(proc);

  if (status)
  {
    sup::sequencer::LogUI ui;
    sup::sequencer::ExecutionStatus exec = sup::sequencer::ExecutionStatus::FAILURE;

    do
    {
      proc->ExecuteSingle(&ui);
      exec = proc->GetStatus();
    } while ((sup::sequencer::ExecutionStatus::SUCCESS != exec)
             && (sup::sequencer::ExecutionStatus::FAILURE != exec));

    status = (sup::sequencer::ExecutionStatus::SUCCESS == exec);
  }

  if (status)
  {
    status = ccs::HelperTools::Exist("/tmp/variable.bck");
  }

  ccs::types::AnyValue value;  // Placeholder

  if (status)
  {
    status = ccs::HelperTools::ReadFromFile(&value, "/tmp/variable.bck");
  }

  if (status)
  {
    status = static_cast<bool>(value.GetType());
  }

  // Test variable
  if (status)
  {
    status =
        (ccs::HelperTools::HasAttribute(&value, "severity")
         && (ccs::types::UnsignedInteger32
             == ccs::HelperTools::GetAttributeType(&value, "severity"))
         && (7u == ccs::HelperTools::GetAttributeValue<ccs::types::uint32>(&value, "severity")));
  }

  Terminate();

  ASSERT_EQ(true, status);
}

#undef LOG_ALTERN_SRC
