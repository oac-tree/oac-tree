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

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/variable_registry.h>
#include <sup/sequencer/variables/local_variable.h>
#include <sup/sequencer/workspace.h>

#include <gmock/gmock.h>
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

/**
 * @brief The TestServerVariable class is a test variable to check setup/teardown actions.
 */
class TestServerVariable : public Variable
{
public:
  TestServerVariable(const SetupTeardownActions& external_action)
      : Variable(Type), m_external_action(external_action)
  {
  }

  ~TestServerVariable() override {}

  bool GetValueImpl(sup::dto::AnyValue&) const override { return true; }
  bool SetValueImpl(const sup::dto::AnyValue&) override { return true; }

private:
  SetupTeardownActions SetupImpl(const Workspace& ws) override { return m_external_action; }

  static const std::string Type;
  SetupTeardownActions m_external_action;
};

const std::string TestServerVariable::Type = "TestServerVariable";

const std::string JSON_TYPE_ATTRIBUTE = "type";
const std::string JSON_VALUE_ATTRIBUTE = "value";

const std::string var1_name = "var1";
const std::string var2_name = "var2";
const std::string var3_name = "var3";

const std::string var2_type =
    R"RAW({"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}},{"status":{"type":"bool"}},{"message":{"type":"string"}}]})RAW";
const std::string var3_type =
    R"RAW({"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}},{"status":{"type":"bool"}},{"message":{"type":"string"}}]})RAW";
const std::string var3_val = R"RAW({"value":55,"status":1})RAW";

TEST_F(WorkspaceTest, DefaultConstructed)
{
  auto variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 0);

  sup::dto::AnyValue val;
  EXPECT_FALSE(ws.GetValue(var1_name, val));
  EXPECT_FALSE(ws.SetValue(var1_name, val));
}

TEST_F(WorkspaceTest, AddVariable)
{
  auto variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 0);

  EXPECT_TRUE(ws.AddVariable(var1_name, std::move(var1)));
  EXPECT_TRUE(ws.AddVariable(var2_name, std::move(var2)));
  EXPECT_TRUE(ws.HasVariable(var1_name));
  EXPECT_TRUE(ws.HasVariable(var2_name));
  variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 2);
  EXPECT_NE(std::find(variables.begin(), variables.end(), var1_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var2_name), variables.end());
  EXPECT_EQ(std::find(variables.begin(), variables.end(), var3_name), variables.end());

  // Test failure of adding variable with duplicate name or address
  EXPECT_THROW(ws.AddVariable(var1_name, std::move(var3)), InvalidOperationException);
}

TEST_F(WorkspaceTest, GetValue)
{
  auto variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 0);

  // Add all variables
  sup::dto::AnyValue val2;
  ws.Setup();
  EXPECT_FALSE(ws.GetValue(var2_name, val2));
  EXPECT_FALSE(ws.SetValue(var2_name, val2));
  EXPECT_TRUE(ws.AddVariable(var1_name, std::move(var1)));
  EXPECT_TRUE(ws.AddVariable(var2_name, std::move(var2)));
  EXPECT_TRUE(ws.AddVariable(var3_name, std::move(var3)));
  variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 3);
  EXPECT_NE(std::find(variables.begin(), variables.end(), var1_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var2_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var3_name), variables.end());
  ws.Teardown();

  // Read complete variable
  ws.Setup();
  EXPECT_FALSE(ws.GetValue(var1_name, val2)) << "GetValue should fail for untyped variable!";
  EXPECT_TRUE(ws.GetValue(var2_name, val2));  // zero-initialized
  sup::dto::uint64 value_field;
  sup::dto::boolean status_field;
  EXPECT_NO_THROW(value_field = val2["value"].As<sup::dto::uint64>());
  EXPECT_NO_THROW(status_field = val2["status"].As<sup::dto::boolean>());
  EXPECT_EQ(value_field, 0ul);
  EXPECT_EQ(status_field, false);
  sup::dto::AnyValue val3;
  EXPECT_TRUE(ws.GetValue(var3_name, val3));  // zero-initialized
  EXPECT_NO_THROW(value_field = val3["value"].As<sup::dto::uint64>());
  EXPECT_NO_THROW(status_field = val3["status"].As<sup::dto::boolean>());
  EXPECT_EQ(value_field, 55ul);
  EXPECT_EQ(status_field, true);

  // Read variable fields
  std::string var3_value_field_name = var3_name + ".value";
  std::string var3_status_field_name = var3_name + ".status";
  sup::dto::AnyValue var3_value_field;
  sup::dto::AnyValue var3_status_field(sup::dto::BooleanType);
  EXPECT_TRUE(ws.GetValue(var3_value_field_name, var3_value_field));
  EXPECT_TRUE(ws.GetValue(var3_status_field_name, var3_status_field));
  value_field = var3_value_field.As<sup::dto::uint64>();
  status_field = var3_status_field.As<sup::dto::boolean>();
  EXPECT_EQ(value_field, 55ul);
  EXPECT_EQ(status_field, true);

  // Read fields with wrong type or unknown field
  std::string var3_message_field_name = var3_name + ".message";
  std::string var3_unknown_field_name = var3_name + ".unknown";
  EXPECT_FALSE(ws.GetValue(var3_message_field_name, var3_status_field));
  EXPECT_FALSE(ws.GetValue(var3_unknown_field_name, var3_status_field));
}

TEST_F(WorkspaceTest, SetValue)
{
  auto variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 0);

  // Add all variables
  sup::dto::AnyValue val2;
  EXPECT_FALSE(ws.GetValue(var2_name, val2));
  EXPECT_FALSE(ws.SetValue(var2_name, val2));
  EXPECT_TRUE(ws.AddVariable(var1_name, std::move(var1)));
  EXPECT_TRUE(ws.AddVariable(var2_name, std::move(var2)));
  EXPECT_TRUE(ws.AddVariable(var3_name, std::move(var3)));
  variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 3);
  EXPECT_NE(std::find(variables.begin(), variables.end(), var1_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var2_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var3_name), variables.end());

  // Set complete variable
  ws.Setup();
  sup::dto::AnyValue val1(sup::dto::BooleanType);
  val1 = true;
  EXPECT_TRUE(ws.SetValue(var1_name, val1));
  EXPECT_TRUE(ws.GetValue(var2_name, val2));
  sup::dto::boolean status_field;
  EXPECT_NO_THROW(status_field = val2["status"].As<sup::dto::boolean>());
  EXPECT_EQ(status_field, false);
  EXPECT_NO_THROW(val2["status"] = true);
  EXPECT_TRUE(ws.SetValue(var2_name, val2));
  EXPECT_TRUE(ws.GetValue(var2_name, val2));
  EXPECT_NO_THROW(status_field = val2["status"].As<sup::dto::boolean>());
  EXPECT_EQ(status_field, true);

  // Set variable fields
  std::string var3_status_field_name = var3_name + ".status";
  sup::dto::AnyValue var3_status_field;
  EXPECT_TRUE(ws.GetValue(var3_status_field_name, var3_status_field));
  status_field = var3_status_field.As<sup::dto::boolean>();
  EXPECT_EQ(status_field, true);
  status_field = false;
  EXPECT_TRUE(ws.SetValue(var3_status_field_name, status_field));
  EXPECT_TRUE(ws.GetValue(var3_status_field_name, var3_status_field));
  status_field = var3_status_field.As<sup::dto::boolean>();
  EXPECT_EQ(status_field, false);

  // Set fields with wrong type or unknown field
  std::string var3_message_field_name = var3_name + ".message";
  std::string var3_unknown_field_name = var3_name + ".unknown";
  EXPECT_FALSE(ws.SetValue(var3_message_field_name, var3_status_field));
  EXPECT_FALSE(ws.GetValue(var3_unknown_field_name, var3_status_field));
}

TEST_F(WorkspaceTest, WaitForVariable)
{
  Workspace workspace;
  EXPECT_TRUE(workspace.GetVariables().empty());
  EXPECT_FALSE(workspace.WaitForVariable("v1", 1.0));

  auto v1 = GlobalVariableRegistry().Create("Local");
  workspace.AddVariable("v1", std::move(v1));
  EXPECT_FALSE(workspace.WaitForVariable("v1", 0.0));

  workspace.Setup();
  EXPECT_TRUE(workspace.WaitForVariable("v1", 0.0));
  EXPECT_FALSE(workspace.WaitForVariable("v1", 0.2, false));
}

TEST_F(WorkspaceTest, GetVariables)
{
  Workspace workspace;
  EXPECT_TRUE(workspace.GetVariables().empty());

  auto v1 = GlobalVariableRegistry().Create("Local");
  auto v2 = GlobalVariableRegistry().Create("Local");

  std::vector<const Variable*> expected({v1.get(), v2.get()});

  workspace.AddVariable("v1", std::move(v1));
  workspace.AddVariable("v2", std::move(v2));

  EXPECT_EQ(workspace.GetVariables(), expected);
}

TEST_F(WorkspaceTest, VariableNamesOrder)
{
  Workspace workspace;
  EXPECT_TRUE(workspace.GetVariables().empty());

  auto v1 = GlobalVariableRegistry().Create("Local");
  auto v2 = GlobalVariableRegistry().Create("Local");
  auto v3 = GlobalVariableRegistry().Create("Local");

  std::vector<std::string> expected({"b", "c", "a"});

  // Add variables in non-lexicographical order
  workspace.AddVariable("b", std::move(v1));
  workspace.AddVariable("c", std::move(v2));
  workspace.AddVariable("a", std::move(v3));

  EXPECT_EQ(workspace.VariableNames(), expected);
}

TEST_F(WorkspaceTest, GetVariablesOrder)
{
  Workspace workspace;
  EXPECT_TRUE(workspace.GetVariables().empty());

  auto v1 = GlobalVariableRegistry().Create("Local");
  auto v2 = GlobalVariableRegistry().Create("Local");
  auto v3 = GlobalVariableRegistry().Create("Local");

  std::vector<const Variable*> expected({v1.get(), v2.get(), v3.get()});

  // Add variables in non-lexicographical order
  workspace.AddVariable("b", std::move(v1));
  workspace.AddVariable("c", std::move(v2));
  workspace.AddVariable("a", std::move(v3));

  EXPECT_EQ(workspace.GetVariables(), expected);
}

TEST_F(WorkspaceTest, GetVariable)
{
  Workspace workspace;
  EXPECT_TRUE(workspace.GetVariables().empty());

  auto v1 = GlobalVariableRegistry().Create("Local");
  auto v2 = GlobalVariableRegistry().Create("Local");
  auto p_v1 = v1.get();
  auto p_v2 = v2.get();

  workspace.AddVariable("v1", std::move(v1));
  workspace.AddVariable("v2", std::move(v2));

  EXPECT_EQ(workspace.GetVariable("v1"), p_v1);
  EXPECT_EQ(workspace.GetVariable("v2"), p_v2);
  EXPECT_EQ(workspace.GetVariable("v3"), nullptr);
}

TEST_F(WorkspaceTest, HasVariable)
{
  auto variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 0);

  EXPECT_FALSE(ws.HasVariable(var1_name));
  EXPECT_FALSE(ws.HasVariable(var2_name));
  EXPECT_FALSE(ws.HasVariable(var3_name));
  EXPECT_TRUE(ws.AddVariable(var1_name, std::move(var1)));
  EXPECT_TRUE(ws.AddVariable(var2_name, std::move(var2)));
  EXPECT_TRUE(ws.HasVariable(var1_name));
  EXPECT_TRUE(ws.HasVariable(var2_name));
  EXPECT_FALSE(ws.HasVariable(var3_name));
}

TEST_F(WorkspaceTest, NotifyCallback)
{
  std::string var_name;
  sup::dto::AnyValue var_value;
  bool var_connected;
  EXPECT_TRUE(ws.RegisterGenericCallback(
      [&](const std::string& name, const sup::dto::AnyValue& value, bool connected)
      {
        var_name = name;
        var_value = value;
        var_connected = connected;
      }));
  std::string name = "FromWorkspace";
  auto var = GlobalVariableRegistry().Create("Local");
  EXPECT_TRUE(var->AddAttribute(JSON_TYPE_ATTRIBUTE, R"RAW({"type":"uint16"})RAW"));
  EXPECT_TRUE(ws.AddVariable(name, std::move(var)));
  ws.Setup();
  sup::dto::AnyValue new_value(sup::dto::UnsignedInteger16Type);
  sup::dto::uint16 raw_value = 123;
  new_value = sup::dto::uint16(raw_value);
  EXPECT_TRUE(ws.SetValue(name, new_value));
  EXPECT_EQ(var_name, name);
  EXPECT_EQ(var_value, raw_value);
  EXPECT_TRUE(var_connected);
}

TEST_F(WorkspaceTest, RegisterType)
{
  std::string struct_type_name = "structured_type_test_name";
  sup::dto::AnyType structtype{
      {{"first", sup::dto::UnsignedInteger16Type}, {"second", sup::dto::Float32Type}},
      struct_type_name};
  EXPECT_TRUE(ws.RegisterType(structtype));
  sup::dto::AnyType array_type(2, sup::dto::StringType, struct_type_name);
  EXPECT_FALSE(ws.RegisterType(array_type));
}

TEST_F(WorkspaceTest, ResetVariable)
{
  auto variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 0);

  // Add all variables
  sup::dto::AnyValue val;
  EXPECT_FALSE(ws.GetValue(var1_name, val));
  EXPECT_TRUE(ws.AddVariable(var1_name, std::move(var1)));
  variables = ws.VariableNames();
  EXPECT_EQ(variables.size(), 1);

  // Set complete variable
  ws.Setup();
  sup::dto::AnyValue val1(sup::dto::BooleanType);
  val1 = true;
  EXPECT_TRUE(ws.SetValue(var1_name, val1));
  EXPECT_TRUE(ws.GetValue(var1_name, val));

  // Reset variable
  EXPECT_TRUE(ws.ResetVariable(var1_name));
  EXPECT_FALSE(ws.GetValue(var1_name, val));

  // Reset non-existing variable
  EXPECT_FALSE(ws.ResetVariable(var2_name));
}

TEST_F(WorkspaceTest, SetupTeardownActions)
{
  const std::string identifier("server_variable");
  ::testing::MockFunction<void()> setup_action;
  ::testing::MockFunction<void()> teardown_action;

  SetupTeardownActions actions{identifier, setup_action.AsStdFunction(),
                               teardown_action.AsStdFunction()};

  Workspace workspace;

  auto var0 = std::unique_ptr<TestServerVariable>(new TestServerVariable(actions));
  auto var1 = std::unique_ptr<TestServerVariable>(new TestServerVariable(actions));

  workspace.AddVariable("var0", std::move(var0));
  workspace.AddVariable("var1", std::move(var1));

  // Two variables should trigger one setup and one teardown action

  EXPECT_CALL(setup_action, Call()).Times(1);
  workspace.Setup(); // trigger expectation

  EXPECT_CALL(teardown_action, Call()).Times(1);
  workspace.Teardown();// trigger expectation
}

WorkspaceTest::WorkspaceTest()
    : ws{}
    , var1{GlobalVariableRegistry().Create("Local")}
    , var2{GlobalVariableRegistry().Create("Local")}
    , var3{GlobalVariableRegistry().Create("Local")}
{
  var2->AddAttribute(JSON_TYPE_ATTRIBUTE, var2_type);
  var3->AddAttribute(JSON_TYPE_ATTRIBUTE, var3_type);
  var3->AddAttribute(JSON_VALUE_ATTRIBUTE, var3_val);
}
