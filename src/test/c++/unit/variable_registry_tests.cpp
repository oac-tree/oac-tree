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


#include <sup/sequencer/variable_registry.h>

#include <sup/sequencer/variables/file_variable.h>
#include <sup/sequencer/variables/local_variable.h>

#include <sup/sequencer/exceptions.h>

#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::sequencer;

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

TEST_F(VariableRegistryTest, RegisterVariable)
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

TEST_F(VariableRegistryTest, RegisterGlobalVariable)
{
  auto &global_reg = GlobalVariableRegistry();
  auto var_names = global_reg.RegisteredVariableNames();
  EXPECT_FALSE(global_reg.IsRegisteredVariableName(TestVariable::Type));
  EXPECT_EQ(std::find(var_names.begin(), var_names.end(), TestVariable::Type), var_names.end());
  EXPECT_THROW(global_reg.Create(TestVariable::Type), InvalidOperationException);

  auto size_before = var_names.size();
  RegisterGlobalVariable<TestVariable>();
  var_names = global_reg.RegisteredVariableNames();
  EXPECT_EQ(var_names.size(), size_before + 1);
  EXPECT_TRUE(global_reg.IsRegisteredVariableName(TestVariable::Type));
  EXPECT_NE(std::find(var_names.begin(), var_names.end(), TestVariable::Type), var_names.end());
  EXPECT_TRUE(static_cast<bool>(global_reg.Create(TestVariable::Type)));
}

VariableRegistryTest::VariableRegistryTest() : empty_reg{} {}

VariableRegistryTest::~VariableRegistryTest() = default;
