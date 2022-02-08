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

#include "InstructionRegistry.h"
#include "SequenceParser.h"
#include "UnitTestHelper.h"

#include <common/AnyValue.h>
#include <gtest/gtest.h>

#include <iostream>

class SequencerParserTest : public ::testing::Test
{
public:

  //! Returns multi-line XML string with Sequencer procedure, where user body
  //! is wrapped into necessary elements. Procedure tag deliberately doesn't contain any schema.
  std::string CreateProcedureString(const std::string &body)
  {
    static const std::string header{R"RAW(
<?xml version="1.0" encoding="UTF-8"?>
<Procedure>)RAW"};

    static const std::string footer{R"RAW(
</Procedure>
)RAW"};

    return header + body + footer;
  }

};

TEST_F(SequencerParserTest, Default)
{
  const std::string body{R"(
    <Sequence>
        <Wait name="First" />
        <Wait name="Second" />
        <Inverter>
            <Wait name="Third" />
        </Inverter>
    </Sequence>
    <Workspace>
    </Workspace>
)"};

  const std::string file_name = "/tmp/sequence_fail.xml";
  ::sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, ::sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = sup::sequencer::ParseProcedureFile(file_name);
  ASSERT_TRUE(proc.get() != nullptr);
}

TEST_F(SequencerParserTest, Workspace)
{
  const std::string body{R"(
    <Sequence>
        <Wait name="Only" />
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"vartype1","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}' />
        <Local name="var2"
               type='{"type":"vartype2","attributes":[{"value":{"type":"string"}}]}' />
        <Local name="var3"
               type='{"type":"vartype3","attributes":[{"value":{"type":"bool"}}]}'/>
    </Workspace>
)"};

  const std::string file_name = "/tmp/workspace.xml";
  ::sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, ::sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = sup::sequencer::ParseProcedureFile(file_name);
  ::sup::UnitTestHelper::PrintProcedureWorkspace(proc.get());
}

TEST_F(SequencerParserTest, ParseString)
{
  const std::string body{R"(
    <Sequence>
        <Wait name="First" />
        <Wait name="Second" />
    </Sequence>
    <Workspace>
    </Workspace>
)"};

  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  ::sup::UnitTestHelper::PrintProcedureWorkspace(proc.get());
}

TEST_F(SequencerParserTest, EmptyProcedureFromXMLAndBack)
{
  auto xml_string = CreateProcedureString(std::string());

  auto procedure = ::sup::sequencer::ParseProcedureString(xml_string);

  std::cout << "XXX>" <<xml_string << "\n";

  EXPECT_EQ(xml_string, ::sup::sequencer::GetXMLString(*procedure));

}
