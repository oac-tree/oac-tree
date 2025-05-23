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

#include <sup/oac-tree/named_callback_manager.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;
using namespace std::placeholders;

class TestCallbackObject
{
  public:
  TestCallbackObject() : value{}, last_name{} {}
  ~TestCallbackObject() = default;

  void IntCallback(int val)
  {
    value += val;
  }

  void NamedIntCallback(const std::string& name, int val)
  {
    last_name = name;
    IntCallback(val);
  }

private:
  int value;
  std::string last_name;
};

class NamedCallbackManagerTest : public ::testing::Test
{
protected:
  NamedCallbackManagerTest();
  ~NamedCallbackManagerTest();
};

TEST_F(NamedCallbackManagerTest, GenericIntCallbacks)
{
  std::string name;
  int value = 0;
  NamedCallbackManager<int> cb_mngr;
  EXPECT_TRUE(cb_mngr.RegisterGenericCallback([&](const std::string& _name, int _value){
    name = _name;
    value = _value;
  }, nullptr));
  EXPECT_EQ(name, std::string{});
  EXPECT_EQ(value, 0);
  std::string new_name = "Updated name";
  int new_value = 1723;
  cb_mngr.ExecuteCallbacks(new_name, new_value);
  EXPECT_EQ(name, new_name);
  EXPECT_EQ(value, new_value);
}

TEST_F(NamedCallbackManagerTest, SpecificIntCallbacks)
{
  std::string name = "MyVarName";
  int value = 0;
  NamedCallbackManager<int> cb_mngr;
  EXPECT_TRUE(cb_mngr.RegisterCallback(name, [&](int _value){
    value = _value;
  }, this));
  EXPECT_EQ(value, 0);
  int val1 = 1723;
  int val2 = -45;
  cb_mngr.ExecuteCallbacks(name, val1);
  EXPECT_EQ(value, val1);
  std::string wrong_name = "OtherName";
  cb_mngr.ExecuteCallbacks(wrong_name, val2);
  EXPECT_EQ(value, val1);
  cb_mngr.ExecuteCallbacks(name, val2);
  EXPECT_EQ(value, val2);
  cb_mngr.UnregisterListener(this);
  cb_mngr.ExecuteCallbacks(name, val1);
  EXPECT_EQ(value, val2);
}

TEST_F(NamedCallbackManagerTest, CallbackGuard)
{
  std::string name = "MyVarName";
  int value = 0;
  std::string current_name;
  NamedCallbackManager<int> cb_mngr;
  {
    auto cb_guard = cb_mngr.GetCallbackGuard(this);
    EXPECT_TRUE(cb_mngr.RegisterGenericCallback([&](const std::string& name, int){
      current_name = name;
    }, this));
    EXPECT_TRUE(cb_mngr.RegisterCallback(
        name, [&](int value_) { value = value_; }, this));
    EXPECT_EQ(value, 0);
    int val1 = 1723;
    cb_mngr.ExecuteCallbacks(name, val1);
    EXPECT_EQ(value, val1);
    EXPECT_EQ(current_name, name);
  }
  int val2 = -45;
  current_name = "";
  cb_mngr.ExecuteCallbacks(name, val2);
  EXPECT_NE(value, val2);
  EXPECT_EQ(current_name, "");
}

NamedCallbackManagerTest::NamedCallbackManagerTest() = default;

NamedCallbackManagerTest::~NamedCallbackManagerTest() = default;
