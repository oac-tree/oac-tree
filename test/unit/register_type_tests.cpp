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

#include "unit_test_helper.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

const std::string RegisterTypeProcedureString{R"(
    <RegisterType jsontype='{"type":"range_uint32","attributes":[{"min":{"type":"uint32"}},{"max":{"type":"uint32"}}]}'/>
    <RegisterType jsontype='{"type":"ranges_uint32","multiplicity":3,"element":{"type":"range_uint32"}}'/>
    <Sequence>
        <Copy name="Copy range variables" inputVar="var1" outputVar="var2" />
        <Copy name="Copy ranges variables" inputVar="var3" outputVar="var4" />
    </Sequence>
    <Workspace>
        <Local name="var1" type='{"type":"range_uint32"}'
               value='{"min":0,"max":10}' />
        <Local name="var2" type='{"type":"range_uint32"}' />
        <Local name="var3" type='{"type":"ranges_uint32"}'
               value='[{"min":0,"max":10},{"min":10,"max":20},{"min":20,"max":40}]' />
        <Local name="var4" type='{"type":"ranges_uint32"}' />
    </Workspace>
)"};

const std::string FailedRegisterTypeProcedureString = {R"(
    <RegisterType jsontype='{"type":"fail_ranges_uint32","multiplicity":3,"element":{"type":"undefined_type"}}'/>
)"};

const std::string RegisterTypeFromFileProcedureString = {R"(
    <RegisterType jsonfile="range_uint16.json"/>
    <RegisterType jsontype='{"type":"ranges_uint16","multiplicity":3,"element":{"type":"range_uint16"}}'/>
    <Sequence>
        <Copy name="Copy range variables" inputVar="var1" outputVar="var2" />
        <Copy name="Copy ranges variables" inputVar="var3" outputVar="var4" />
    </Sequence>
    <Workspace>
        <Local name="var1" type='{"type":"range_uint16"}'
               value='{"min":0,"max":10}' />
        <Local name="var2" type='{"type":"range_uint16"}' />
        <Local name="var3" type='{"type":"ranges_uint16"}'
               value='[{"min":0,"max":10},{"min":10,"max":20},{"min":20,"max":40}]' />
        <Local name="var4" type='{"type":"ranges_uint16"}' />
    </Workspace>
)"};


const std::string FailedRegisterTypeFromFileProcedureString = {R"(
    <RegisterType jsonfile="does_not_exist.json"/>
)"};

const std::string JSON_FILE_NAME = "range_uint16.json";

const std::string JSONRangeRepresentation =
    R"RAW(
{
    "type": "range_uint16",
    "attributes": [
        {
            "min":
            {
                "type": "uint16"
            }
        },
        {
            "max":
            {
                "type": "uint16"
            }
        }
    ]
}
)RAW";

TEST(RegisterType, string_success)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc_str = sup::UnitTestHelper::CreateProcedureString(RegisterTypeProcedureString);
  auto proc = ParseProcedureString(proc_str);
  ASSERT_TRUE(static_cast<bool>(proc));

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(RegisterType, string_failed)
{
  auto proc_str = sup::UnitTestHelper::CreateProcedureString(FailedRegisterTypeProcedureString);
  auto proc = ParseProcedureString(proc_str);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_THROW(proc->Setup(), ProcedureSetupException);
}

TEST(RegisterType, file_success)
{
  sup::UnitTestHelper::TemporaryTestFile json_file(JSON_FILE_NAME, JSONRangeRepresentation);
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc_str = sup::UnitTestHelper::CreateProcedureString(RegisterTypeFromFileProcedureString);
  auto proc = ParseProcedureString(proc_str);
  ASSERT_TRUE(static_cast<bool>(proc));

  // Check if preamble contains correct registration info
  auto type_registrations = proc->GetPreamble().GetTypeRegistrations();
  EXPECT_EQ(type_registrations.size(), 2);
  {
    auto& type_registration = type_registrations[0];
    EXPECT_EQ(type_registration.GetRegistrationMode(), TypeRegistrationInfo::kJSONFile);
    EXPECT_EQ(type_registration.GetString(), "range_uint16.json");
  }
  {
    auto& type_registration = type_registrations[1];
    EXPECT_EQ(type_registration.GetRegistrationMode(), TypeRegistrationInfo::kJSONString);
    std::string expected =
        R"({"type":"ranges_uint16","multiplicity":3,"element":{"type":"range_uint16"}})";
    EXPECT_EQ(type_registration.GetString(), expected);
  }

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(RegisterType, file_failed)
{
  auto proc_str = sup::UnitTestHelper::CreateProcedureString(
    FailedRegisterTypeFromFileProcedureString);
  auto proc = ParseProcedureString(proc_str);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_THROW(proc->Setup(), ProcedureSetupException);
}
