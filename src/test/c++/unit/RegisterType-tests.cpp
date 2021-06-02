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

// Global header files

#include <gtest/gtest.h>

#include <SequenceParser.h>

// Local header files

#include "ExecutionStatus.h"
#include "InstructionRegistry.h"

#include "UnitTestHelper.h"
#include "LogUI.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

// Function declaration

// Global variables

static ccs::log::Func_t _log_handler = ccs::log::SetStdout();

static const std::string RegisterTypeProcedureString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="RegisterType procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <RegisterType jsontype='{"type":"range_uint32","attributes":[{"min":{"type":"uint32"}},{"max":{"type":"uint32"}}]}'/>
    <RegisterType jsontype='{"type":"ranges_uint32","multiplicity":3,"element":{"type":"range_uint32"}}'/>
    <Sequence>
        <Copy name="Copy range variables" input="var1" output="var2" />
        <Copy name="Copy ranges variables" input="var3" output="var4" />
    </Sequence>
    <Workspace>
        <Local name="var1" type='{"type":"range_uint32"}'
               value='{"min":0,"max":10}' />
        <Local name="var2" type='{"type":"range_uint32"}' />
        <Local name="var3" type='{"type":"ranges_uint32"}'
               value='[{"min":0,"max":10},{"min":10,"max":20},{"min":20,"max":40}]' />
        <Local name="var4" type='{"type":"ranges_uint32"}' />
    </Workspace>
</Procedure>
)RAW";

static const std::string FailedRegisterTypeProcedureString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Failing RegisterType procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <RegisterType jsontype='{"type":"fail_ranges_uint32","multiplicity":3,"element":{"type":"undefined_type"}}'/>
</Procedure>
)RAW";

static const std::string RegisterTypeFromFileProcedureString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="RegisterType procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <RegisterType jsonfile="/tmp/range_uint16.json"/>
    <RegisterType jsontype='{"type":"ranges_uint16","multiplicity":3,"element":{"type":"range_uint16"}}'/>
    <Sequence>
        <Copy name="Copy range variables" input="var1" output="var2" />
        <Copy name="Copy ranges variables" input="var3" output="var4" />
    </Sequence>
    <Workspace>
        <Local name="var1" type='{"type":"range_uint16"}'
               value='{"min":0,"max":10}' />
        <Local name="var2" type='{"type":"range_uint16"}' />
        <Local name="var3" type='{"type":"ranges_uint16"}'
               value='[{"min":0,"max":10},{"min":10,"max":20},{"min":20,"max":40}]' />
        <Local name="var4" type='{"type":"ranges_uint16"}' />
    </Workspace>
</Procedure>
)RAW";

static const std::string FailedRegisterTypeFromFileProcedureString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Failing RegisterType procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <RegisterType jsonfile="does_not_exist.json"/>
</Procedure>
)RAW";

static const std::string JSON_FILE_NAME = "/tmp/range_uint16.json";

static const std::string JSONRangeRepresentation =
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

// Function definition

TEST(RegisterType, string_success)
{
  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(RegisterTypeProcedureString);
  ASSERT_TRUE(static_cast<bool>(proc));

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}

TEST(RegisterType, string_failed)
{
  auto proc = sup::sequencer::ParseProcedureString(FailedRegisterTypeProcedureString);
  EXPECT_FALSE(static_cast<bool>(proc));
}

TEST(RegisterType, file_success)
{
  sup::UnitTestHelper::TemporaryTestFile json_file(JSON_FILE_NAME, JSONRangeRepresentation);
  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(RegisterTypeFromFileProcedureString);
  ASSERT_TRUE(static_cast<bool>(proc));

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}

TEST(RegisterType, file_failed)
{
  auto proc = sup::sequencer::ParseProcedureString(FailedRegisterTypeFromFileProcedureString);
  EXPECT_FALSE(static_cast<bool>(proc));
}

#undef LOG_ALTERN_SRC
