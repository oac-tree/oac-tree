/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : Rui Oliveira (EXT)
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

#include "unit_test_helper.h"

#include <gtest/gtest.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>

using namespace sup::oac_tree;

TEST(AddElement, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("AddElement");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
  EXPECT_TRUE(instr->AddAttribute("inputVar", "input_var"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
  EXPECT_TRUE(instr->AddAttribute("outputVar", "output_var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(AddElement, AddedUint8Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"uint8"}'
               value='28'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"uint8"}}'
               value='[0]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, AddedInt8Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"int8"}'
               value='28'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"int8"}}'
               value='[0]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, AddedUint16Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"uint16"}'
               value='28'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"uint16"}}'
               value='[0]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, AddedInt16Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"int16"}'
               value='28'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"int16"}}'
               value='[0]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, AddedUint32Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"uint32"}'
               value='28'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"uint32"}}'
               value='[0]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, AddedInt32Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"int32"}'
               value='28'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"int32"}}'
               value='[0]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, AddedUint64Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"uint64"}'
               value='28'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"uint64"}}'
               value='[0]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, AddedInt64Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"int64"}'
               value='28'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"int64"}}'
               value='[0]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, AddedFloat32Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"float32"}'
               value='28'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"float32"}}'
               value='[28.24]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, AddedFloat64Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"float64"}'
               value='28.0'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"float64"}}'
               value='[28.24]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, AddedChar8Successfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"char8"}'
               value='28.0'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"char8"}}'
               value='[28.24]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, AddedBoolSuccessfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"bool"}'
               value='true'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"bool"}}'
               value='[false]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, AddedComplexStructureSuccessfully)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"struct",
                      "attributes":
                          [{"id":{"type":"int8"}},
                           {"map":{"type":"array",
                                   "multiplicity":1,
                                   "element":{"type":"struct",
                                              "attributes":[{"name":{"type":"string"}},
                                                            {"id":{"type":"int8"}}]}}},
                           {"comms":{"type":"struct",
                                      "attributes":[{"port":{"type":"int8"}},
                                                    {"ip":{"type":"string"}}]}}]}'
               value='{"id":0,"map":[{"name":"IPs","id":0}],"comms":{"port":0,"ip":"1.0.0.1"}}'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array",
                      "multiplicity":1,
                      "element":
                          {"type":"struct",
                           "attributes":
                             [{"id":{"type":"int8"}},
                              {"map":{"type":"array",
                                      "multiplicity":1,
                                      "element":{"type":"struct",
                                                 "attributes":[{"name":{"type":"string"}},
                                                               {"id":{"type":"int8"}}]}}},
                              {"comms":{"type":"struct",
                                        "attributes":[{"port":{"type":"int8"}},
                                                      {"ip":{"type":"string"}}]}}]}}'
               value='[{"id":0,"map":[{"name":"IPs","id":0}],"comms":{"port":0,"ip":"1.0.0.1"}}]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, FailMismatchedType)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"char8"}'
               value='28.0'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"uint32"}}'
               value='[28.24]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, FailMismatchedComplexStructureType)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"struct",
                      "attributes":
                          [{"id":{"type":"int8"}},
                           {"map":{"type":"array",
                                   "multiplicity":1,
                                   "element":{"type":"struct",
                                              "attributes":[{"name":{"type":"string"}},
                                                            {"id":{"type":"int8"}}]}}},
                           {"comms":{"type":"struct",
                                      "attributes":[{"port":{"type":"int8"}},
                                                    {"ip":{"type":"string"}}]}}]}'
               value='{"id":0,"map":[{"name":"IPs","id":0}],"comms":{"port":0,"ip":"1.0.0.1"}}'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array",
                      "multiplicity":1,
                      "element":
                          {"type":"struct",
                           "attributes":[{"map":
                                            {"type":"array",
                                             "multiplicity":1,
                                             "element":{"type":"struct",
                                                        "attributes":[{"name":{"type":"string"}},
                                                                      {"id":{"type":"int8"}}]}}},
                                         {"comms":{"type":"struct",
                                                   "attributes":[{"port":{"type":"int8"}},
                                                                 {"ip":{"type":"string"}}]}}]}}'
               value='[{"map":[{"name":"IPs","id":0}],"comms":{"port":0,"ip":"1.0.0.1"}}]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, FailTypeLockedDynamicTypeExplicitlyDefined)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"char8"}'
               value='28.0'/>
        <Local name="var2"
               dynamicType="false"
               type='{"type":"array","multiplicity":1,"element":{"type":"char8"}}'
               value='[28.24]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(AddElement, FailTypeLockedDynamicTypeImplicitlyDefined)
{
  const std::string body{
      R"(
    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               dynamicType="true"
               type='{"type":"char8"}'
               value='28.0'/>
        <Local name="var2"
               type='{"type":"array","multiplicity":1,"element":{"type":"char8"}}'
               value='[28.24]'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}
