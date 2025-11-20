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


#include <sup/oac-tree/variable_registry.h>
#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/variables/file_variable.h>
#include <sup/oac-tree/variables/local_variable.h>

#include <sup/oac-tree/exceptions.h>

#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::oac_tree;

class VariableRegistryTest : public ::testing::Test
{
protected:
  VariableRegistryTest();
  virtual ~VariableRegistryTest();

  VariableRegistry empty_reg;
};

class TestVariable : public Variable
{
private:
  bool GetValueImpl(sup::dto::AnyValue &) const override { return true; }
  bool SetValueImpl(const sup::dto::AnyValue &) override { return true; }

public:
  TestVariable() : Variable{Type} {}
  ~TestVariable() override {}

  static const std::string Type;
};

template <>
DocumentationEntry sup::oac_tree::VariableDocFor<TestVariable>()
{
  DocumentationEntry doc{{
    { Constants::VAR_DOC_DESCRIPTION, "TEST" }
  }};
  return doc;
}

const std::string TestVariable::Type = "TestVariable";

TEST_F(VariableRegistryTest, DefaultConstructed)
{
  auto var_names = empty_reg.RegisteredVariableNames();
  EXPECT_EQ(var_names.size(), 0);
  EXPECT_THROW(empty_reg.Create("VariableTypeDoesNotExist"), InvalidOperationException);
}

TEST_F(VariableRegistryTest, GlobalRegistry)
{
  auto &global_reg = GlobalVariableRegistry();
  auto var_names = global_reg.RegisteredVariableNames();
  EXPECT_GE(var_names.size(), 2);
  EXPECT_TRUE(static_cast<bool>(global_reg.Create(FileVariable::Type)));
}

TEST_F(VariableRegistryTest, EmptyRegistry)
{
  auto var_names = empty_reg.RegisteredVariableNames();
  EXPECT_EQ(var_names.size(), 0);
  EXPECT_THROW(empty_reg.Create(FileVariable::Type), InvalidOperationException);
  EXPECT_TRUE(RegisterVariable<FileVariable>(empty_reg));
  var_names = empty_reg.RegisteredVariableNames();
  EXPECT_EQ(var_names.size(), 1);
  EXPECT_TRUE(empty_reg.IsRegisteredVariableName(FileVariable::Type));
  EXPECT_NE(std::find(var_names.begin(), var_names.end(), FileVariable::Type), var_names.end());
  EXPECT_TRUE(static_cast<bool>(empty_reg.Create(FileVariable::Type)));
}

TEST_F(VariableRegistryTest, RegisterTestVariable)
{
  // copy global registry
  auto registry = GlobalVariableRegistry();
  auto var_names = registry.RegisteredVariableNames();
  EXPECT_FALSE(registry.IsRegisteredVariableName(TestVariable::Type));
  EXPECT_EQ(std::find(var_names.begin(), var_names.end(), TestVariable::Type), var_names.end());
  EXPECT_THROW(registry.Create(TestVariable::Type), InvalidOperationException);

  auto size_before = var_names.size();
  RegisterVariable<TestVariable>(registry);
  var_names = registry.RegisteredVariableNames();
  EXPECT_EQ(var_names.size(), size_before + 1);
  EXPECT_TRUE(registry.IsRegisteredVariableName(TestVariable::Type));
  EXPECT_NE(std::find(var_names.begin(), var_names.end(), TestVariable::Type), var_names.end());
  EXPECT_TRUE(static_cast<bool>(registry.Create(TestVariable::Type)));

  // Documentation object is as expected
  const auto doc = registry.GetDocumentation(TestVariable::Type);
  const auto doc_fields = doc.GetFieldNames();
  ASSERT_EQ(doc_fields.size(), 1);
  EXPECT_EQ(doc_fields[0], Constants::VAR_DOC_DESCRIPTION);
  EXPECT_EQ(doc.GetField(Constants::VAR_DOC_DESCRIPTION), "TEST");
}

TEST_F(VariableRegistryTest, BaseVariables)
{
  const auto registered_names = GlobalVariableRegistry().RegisteredVariableNames();
  for (const auto& var_name : registered_names)
  {
    // can create variable
    auto inst = GlobalVariableRegistry().Create(var_name);
    EXPECT_TRUE(static_cast<bool>(inst));

    // Documentation object is as expected
    const auto doc = GlobalVariableRegistry().GetDocumentation(var_name);
    const auto doc_fields = doc.GetFieldNames();
    ASSERT_EQ(doc_fields.size(), 1) << "Incorrect number of doc entries for variable "
      << var_name;
    EXPECT_EQ(doc_fields[0], Constants::VAR_DOC_DESCRIPTION);
  }
}

VariableRegistryTest::VariableRegistryTest() : empty_reg{} {}

VariableRegistryTest::~VariableRegistryTest() = default;
