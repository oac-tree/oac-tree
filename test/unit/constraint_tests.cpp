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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/sequencer/concrete_constraints.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

const std::string kEmptyStringAttrName = "empty_string";
const std::string kDoubleAttrName = "double";

class TestConstraint : public IConstraint
{
public:
  TestConstraint(bool pass) : m_pass{pass} {}
  ~TestConstraint() = default;

  TestConstraint* Clone() const override { return new TestConstraint{m_pass}; }

  bool Validate(const StringAttributeList& attr_map) const override { return m_pass; }
  std::string GetRepresentation() const override
  {
    std::string pass_str = m_pass ? "true" : "false";
    return "TestConstraint(" + pass_str + ")";
  }
private:
  bool m_pass;
};

class ConstraintTest : public ::testing::Test
{
protected:
  ConstraintTest();
  virtual ~ConstraintTest();

  StringAttributeList m_attr_map;
};

TEST_F(ConstraintTest, Exists)
{
  {
    // Exists passes when the attribute is present in the given map
    auto constraint = MakeConstraint<Exists>(kEmptyStringAttrName);
    EXPECT_TRUE(constraint.Validate(m_attr_map));
    EXPECT_FALSE(constraint.GetRepresentation().empty());
  }
  {
    // Exists fails when the attribute is not present in the given map
    auto constraint = MakeConstraint<Exists>("does_not_exist");
    EXPECT_FALSE(constraint.Validate(m_attr_map));
    EXPECT_FALSE(constraint.GetRepresentation().empty());
  }
  {
    // Copy
    auto constraint = MakeConstraint<Exists>(kEmptyStringAttrName);
    auto repr = constraint.GetRepresentation();
    Constraint copy_constructed{constraint};
    EXPECT_EQ(copy_constructed.GetRepresentation(), repr);
    auto copy_assigned = MakeConstraint<TestConstraint>(true);
    copy_assigned = constraint;
    EXPECT_EQ(copy_constructed.GetRepresentation(), repr);
  }
  {
    // Move
    auto constraint = MakeConstraint<Exists>(kEmptyStringAttrName);
    auto repr = constraint.GetRepresentation();
    EXPECT_TRUE(constraint.Validate(m_attr_map));
    Constraint move_constructed{std::move(constraint)};
    // Move source always fails validation during construction
    EXPECT_FALSE(constraint.Validate(m_attr_map));
    EXPECT_EQ(move_constructed.GetRepresentation(), repr);
    EXPECT_TRUE(move_constructed.Validate(m_attr_map));
    auto move_assigned = MakeConstraint<TestConstraint>(true);
    move_assigned = std::move(move_constructed);
    EXPECT_EQ(move_assigned.GetRepresentation(), repr);
    EXPECT_TRUE(move_assigned.Validate(m_attr_map));
  }
}

TEST_F(ConstraintTest, Either)
{
  {
    // Validation passes when exactly one child constraint passes.
    auto constraint = MakeConstraint<Either>(MakeConstraint<Exists>(kEmptyStringAttrName),
                                             MakeConstraint<Exists>("does_not_exist"));
    EXPECT_TRUE(constraint.Validate(m_attr_map));
    EXPECT_FALSE(constraint.GetRepresentation().empty());
  }
  {
    // Validation passes when exactly one child constraint passes.
    auto constraint = MakeConstraint<Either>(MakeConstraint<Exists>("does_not_exist"),
                                             MakeConstraint<Exists>(kEmptyStringAttrName));
    EXPECT_TRUE(constraint.Validate(m_attr_map));
    EXPECT_FALSE(constraint.GetRepresentation().empty());
  }
  {
    // Validation fails when no child constraint passes.
    auto constraint = MakeConstraint<Either>(MakeConstraint<Exists>("does_not_exist"),
                                             MakeConstraint<Exists>("does_not_exist"));
    EXPECT_FALSE(constraint.Validate(m_attr_map));
    EXPECT_FALSE(constraint.GetRepresentation().empty());
  }
  {
    // Validation fails when both child constraints pass.
    auto constraint = MakeConstraint<Either>(MakeConstraint<Exists>(kEmptyStringAttrName),
                                             MakeConstraint<Exists>(kDoubleAttrName));
    EXPECT_FALSE(constraint.Validate(m_attr_map));
    EXPECT_FALSE(constraint.GetRepresentation().empty());
  }
  {
    // Copy
    auto constraint = MakeConstraint<Either>(MakeConstraint<Exists>("does_not_exist"),
                                             MakeConstraint<Exists>(kEmptyStringAttrName));
    auto repr = constraint.GetRepresentation();
    Constraint copy_constructed{constraint};
    EXPECT_EQ(copy_constructed.GetRepresentation(), repr);
    auto copy_assigned = MakeConstraint<TestConstraint>(true);
    copy_assigned = constraint;
    EXPECT_EQ(copy_constructed.GetRepresentation(), repr);
  }
  {
    // Move
    auto constraint = MakeConstraint<Either>(MakeConstraint<Exists>("does_not_exist"),
                                             MakeConstraint<Exists>(kEmptyStringAttrName));
    auto repr = constraint.GetRepresentation();
    EXPECT_TRUE(constraint.Validate(m_attr_map));
    Constraint move_constructed{std::move(constraint)};
    // Move source always fails validation
    EXPECT_FALSE(constraint.Validate(m_attr_map));
    EXPECT_EQ(move_constructed.GetRepresentation(), repr);
    EXPECT_TRUE(move_constructed.Validate(m_attr_map));
    auto move_assigned = MakeConstraint<TestConstraint>(true);
    move_assigned = std::move(move_constructed);
    EXPECT_EQ(move_assigned.GetRepresentation(), repr);
    EXPECT_TRUE(move_assigned.Validate(m_attr_map));
  }
}

TEST_F(ConstraintTest, Both)
{
  {
    // Validation passes when both child constraints pass.
    auto constraint = MakeConstraint<Both>(MakeConstraint<Exists>(kEmptyStringAttrName),
                                           MakeConstraint<Exists>(kDoubleAttrName));
    EXPECT_TRUE(constraint.Validate(m_attr_map));
    EXPECT_FALSE(constraint.GetRepresentation().empty());
  }
  {
    // Validation fails when only one child constraint passes.
    auto constraint = MakeConstraint<Both>(MakeConstraint<Exists>(kEmptyStringAttrName),
                                           MakeConstraint<Exists>("does_not_exist"));
    EXPECT_FALSE(constraint.Validate(m_attr_map));
    EXPECT_FALSE(constraint.GetRepresentation().empty());
  }
  {
    // Validation fails when only one child constraint passes.
    auto constraint = MakeConstraint<Both>(MakeConstraint<Exists>("does_not_exist"),
                                           MakeConstraint<Exists>(kEmptyStringAttrName));
    EXPECT_FALSE(constraint.Validate(m_attr_map));
    EXPECT_FALSE(constraint.GetRepresentation().empty());
  }
  {
    // Validation fails when no child constraint passes.
    auto constraint = MakeConstraint<Both>(MakeConstraint<Exists>("does_not_exist"),
                                           MakeConstraint<Exists>("does_not_exist"));
    EXPECT_FALSE(constraint.Validate(m_attr_map));
    EXPECT_FALSE(constraint.GetRepresentation().empty());
  }
  {
    // Copy
    auto constraint = MakeConstraint<Both>(MakeConstraint<Exists>(kEmptyStringAttrName),
                                           MakeConstraint<Exists>(kDoubleAttrName));
    auto repr = constraint.GetRepresentation();
    Constraint copy_constructed{constraint};
    EXPECT_EQ(copy_constructed.GetRepresentation(), repr);
    auto copy_assigned = MakeConstraint<TestConstraint>(true);
    copy_assigned = constraint;
    EXPECT_EQ(copy_constructed.GetRepresentation(), repr);
  }
  {
    // Move
    auto constraint = MakeConstraint<Both>(MakeConstraint<Exists>(kEmptyStringAttrName),
                                           MakeConstraint<Exists>(kDoubleAttrName));
    auto repr = constraint.GetRepresentation();
    EXPECT_TRUE(constraint.Validate(m_attr_map));
    Constraint move_constructed{std::move(constraint)};
    // Move source always fails validation
    EXPECT_FALSE(constraint.Validate(m_attr_map));
    EXPECT_EQ(move_constructed.GetRepresentation(), repr);
    EXPECT_TRUE(move_constructed.Validate(m_attr_map));
    auto move_assigned = MakeConstraint<TestConstraint>(true);
    move_assigned = std::move(move_constructed);
    EXPECT_EQ(move_assigned.GetRepresentation(), repr);
    EXPECT_TRUE(move_assigned.Validate(m_attr_map));
  }
}

ConstraintTest::ConstraintTest()
{
  m_attr_map.emplace_back(kEmptyStringAttrName, "");
  m_attr_map.emplace_back(kDoubleAttrName, "1.0");
}

ConstraintTest::~ConstraintTest() = default;
