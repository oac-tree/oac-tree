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

#include "LocalVariable.h"
#include "SequenceParser.h"
#include "VariableRegistry.h"
#include "Workspace.h"

#include <common/AnyValueHelper.h>
#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::sequencer;

class WorkspaceTest : public ::testing::Test
{
protected:
  WorkspaceTest();

  Workspace ws;
  std::unique_ptr<Variable> var1;
  std::unique_ptr<Variable> var2;
  std::unique_ptr<Variable> var3;
};

static const std::string var1_name = "var1";
static const std::string var2_name = "var2";
static const std::string var3_name = "var3";

static const std::string var2_type =
    R"RAW({"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}},{"status":{"type":"bool"}}]})RAW";
static const std::string var3_type =
    R"RAW({"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}},{"status":{"type":"bool"}}]})RAW";
static const std::string var3_val = R"RAW({"value":55,"status":1})RAW";

// Function definition

TEST_F(WorkspaceTest, DefaultConstructed)
{
  auto variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 0);

  ::ccs::types::AnyValue val;
  EXPECT_FALSE(ws.GetValue(var1_name, val));
  EXPECT_FALSE(ws.SetValue(var1_name, val));
}

TEST_F(WorkspaceTest, AddVariable)
{
  auto variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 0);

  Variable *var1_address = var1.get();  // cache address for later
  EXPECT_TRUE(ws.AddVariable(var1_name, var1.release()));
  EXPECT_TRUE(ws.AddVariable(var2_name, var2.release()));
  variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 2);
  EXPECT_NE(std::find(variables.begin(), variables.end(), var1_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var2_name), variables.end());
  EXPECT_EQ(std::find(variables.begin(), variables.end(), var3_name), variables.end());

  // Test failure of adding variable with duplicate name or address
  EXPECT_FALSE(ws.AddVariable(var1_name, var3.release()));
  std::string var4_name = "var4";
  EXPECT_FALSE(ws.AddVariable(var4_name, var1_address));
}

TEST_F(WorkspaceTest, GetValue)
{
  auto variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 0);

  // Add all variables
  ::ccs::types::AnyValue val2;
  EXPECT_FALSE(ws.GetValue(var2_name, val2));
  EXPECT_FALSE(ws.SetValue(var2_name, val2));
  EXPECT_TRUE(ws.AddVariable(var1_name, var1.release()));
  EXPECT_TRUE(ws.AddVariable(var2_name, var2.release()));
  EXPECT_TRUE(ws.AddVariable(var3_name, var3.release()));
  variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 3);
  EXPECT_NE(std::find(variables.begin(), variables.end(), var1_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var2_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var3_name), variables.end());

  // Read complete variable
  EXPECT_FALSE(ws.GetValue(var1_name, val2)) << "GetValue should fail for untypes variable!";
  EXPECT_TRUE(ws.GetValue(var2_name, val2));  // zero-initialized
  ::ccs::types::uint64 value_field;
  ::ccs::types::boolean status_field;
  EXPECT_TRUE(::ccs::HelperTools::GetAttributeValue(&val2, "value", value_field));
  EXPECT_TRUE(::ccs::HelperTools::GetAttributeValue(&val2, "status", status_field));
  EXPECT_EQ(value_field, 0ul);
  EXPECT_EQ(status_field, false);
  ::ccs::types::AnyValue val3;
  EXPECT_TRUE(ws.GetValue(var3_name, val3));  // zero-initialized
  EXPECT_TRUE(::ccs::HelperTools::GetAttributeValue(&val3, "value", value_field));
  EXPECT_TRUE(::ccs::HelperTools::GetAttributeValue(&val3, "status", status_field));
  EXPECT_EQ(value_field, 55ul);
  EXPECT_EQ(status_field, true);

  // Read variable fields
  std::string var3_value_field_name = var3_name + ".value";
  std::string var3_status_field_name = var3_name + ".status";
  ::ccs::types::AnyValue var3_value_field;
  ::ccs::types::AnyValue var3_status_field(::ccs::types::Boolean);
  EXPECT_TRUE(ws.GetValue(var3_value_field_name, var3_value_field));
  EXPECT_TRUE(ws.GetValue(var3_status_field_name, var3_status_field));
  value_field = var3_value_field;
  status_field = var3_status_field;
  EXPECT_EQ(value_field, 55ul);
  EXPECT_EQ(status_field, true);

  // Read fields with wrong size or unknown field
  std::string var3_unknown_field_name = var3_name + ".unknown";
  EXPECT_FALSE(ws.GetValue(var3_value_field_name, var3_status_field));
  EXPECT_FALSE(ws.GetValue(var3_unknown_field_name, var3_status_field));
}

TEST_F(WorkspaceTest, SetValue)
{
  auto variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 0);

  // Add all variables
  ::ccs::types::AnyValue val2;
  EXPECT_FALSE(ws.GetValue(var2_name, val2));
  EXPECT_FALSE(ws.SetValue(var2_name, val2));
  EXPECT_TRUE(ws.AddVariable(var1_name, var1.release()));
  EXPECT_TRUE(ws.AddVariable(var2_name, var2.release()));
  EXPECT_TRUE(ws.AddVariable(var3_name, var3.release()));
  variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 3);
  EXPECT_NE(std::find(variables.begin(), variables.end(), var1_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var2_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var3_name), variables.end());

  // Set complete variable
  ::ccs::types::AnyValue val1(::ccs::types::Boolean);
  val1 = true;
  EXPECT_TRUE(ws.SetValue(var1_name, val1));
  EXPECT_TRUE(ws.GetValue(var2_name, val2));  // zero-initialized
  ::ccs::types::boolean status_field;
  EXPECT_TRUE(::ccs::HelperTools::GetAttributeValue(&val2, "status", status_field));
  EXPECT_EQ(status_field, false);
  EXPECT_TRUE(::ccs::HelperTools::SetAttributeValue(&val2, "status", true));
  EXPECT_TRUE(ws.SetValue(var2_name, val2));
  EXPECT_TRUE(ws.GetValue(var2_name, val2));  // zero-initialized
  EXPECT_TRUE(::ccs::HelperTools::GetAttributeValue(&val2, "status", status_field));
  EXPECT_EQ(status_field, true);

  // Set variable fields
  std::string var3_status_field_name = var3_name + ".status";
  ::ccs::types::AnyValue var3_status_field;
  EXPECT_TRUE(ws.GetValue(var3_status_field_name, var3_status_field));
  status_field = var3_status_field;
  EXPECT_EQ(status_field, true);
  status_field = false;
  EXPECT_TRUE(ws.SetValue(var3_status_field_name, status_field));
  EXPECT_TRUE(ws.GetValue(var3_status_field_name, var3_status_field));
  status_field = var3_status_field;
  EXPECT_EQ(status_field, false);

  // Set fields with wrong size or unknown field
  std::string var3_value_field_name = var3_name + ".value";
  std::string var3_unknown_field_name = var3_name + ".unknown";
  EXPECT_FALSE(ws.SetValue(var3_value_field_name, var3_status_field));
  EXPECT_FALSE(ws.GetValue(var3_unknown_field_name, var3_status_field));
}

TEST_F(WorkspaceTest, GetVariables)
{
  Workspace workspace;
  EXPECT_TRUE(workspace.GetVariables().empty());

  auto v1 = GlobalVariableRegistry().Create("Local");
  auto v2 = GlobalVariableRegistry().Create("Local");

  std::vector<const Variable *> expected({v1.get(), v2.get()});

  workspace.AddVariable("v1", v1.release());
  workspace.AddVariable("v2", v2.release());

  EXPECT_EQ(workspace.GetVariables(), expected);
}

TEST_F(WorkspaceTest, NotifyCallback)
{
  std::string var_name;
  ccs::types::AnyValue var_value;
  EXPECT_TRUE(ws.RegisterGenericCallback(
    [&var_name, &var_value](const std::string& name, const ccs::types::AnyValue& value)
    {
      var_name = name;
      var_value = value;
    }));
  std::string name = "FromWorkspace";
  auto var = GlobalVariableRegistry().Create("Local");
  EXPECT_TRUE(var->AddAttribute(LocalVariable::JSON_TYPE, R"RAW({"type":"uint16"})RAW"));
  EXPECT_TRUE(ws.AddVariable(name, var.release()));
  ccs::types::AnyValue new_value(ccs::types::UnsignedInteger16);
  ccs::types::uint16 raw_value = 123;
  new_value = ccs::types::uint16(raw_value);
  EXPECT_TRUE(ws.SetValue(name, new_value));
  EXPECT_EQ(var_name, name);
  EXPECT_EQ(var_value, raw_value);
}

WorkspaceTest::WorkspaceTest()
    : ws{}
    , var1{GlobalVariableRegistry().Create("Local")}
    , var2{GlobalVariableRegistry().Create("Local")}
    , var3{GlobalVariableRegistry().Create("Local")}
{
  var2->AddAttribute(LocalVariable::JSON_TYPE, var2_type);
  var3->AddAttribute(LocalVariable::JSON_TYPE, var3_type);
  var3->AddAttribute(LocalVariable::JSON_VALUE, var3_val);
}
