/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
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

#ifndef SUP_OAC_TREE_CONCRETE_CONSTRAINTS_H_
#define SUP_OAC_TREE_CONCRETE_CONSTRAINTS_H_

#include <sup/oac-tree/constraint.h>

namespace sup
{
namespace oac_tree
{

/**
 * Constraint that checks presence of mandatory attribute.
*/
class Exists : public IConstraint
{
public:
  explicit Exists(const std::string& attr_name);
  ~Exists();

  Exists* Clone() const override;

  bool Validate(const StringAttributeList& attr_map) const override;
  std::string GetRepresentation() const override;
private:
  const std::string m_attr_name;
};

/**
 * Constraint that checks if attribute value can represent the correct type.
*/
class FixedType : public IConstraint
{
public:
  explicit FixedType(const std::string& attr_name, const sup::dto::AnyType& attr_type);
  ~FixedType();

  FixedType* Clone() const override;

  bool Validate(const StringAttributeList& attr_map) const override;
  std::string GetRepresentation() const override;
private:
  const std::string m_attr_name;
  const sup::dto::AnyType m_attr_type;
};

/**
 * Composite constraint that checks if either the left or the right constraint is satisfied.
 * This is equivalent to an 'XOR' operation.
*/
class Xor : public IConstraint
{
public:
  explicit Xor(Constraint&& left, Constraint&& right);
  ~Xor();

  Xor* Clone() const override;

  bool Validate(const StringAttributeList& attr_map) const override;
  std::string GetRepresentation() const override;
private:
  Constraint m_left;
  Constraint m_right;
};

/**
 * Composite constraint that checks if the left or the right constraint is satisfied (or both).
 * This is equivalent to an 'OR' operation.
*/
class Or : public IConstraint
{
public:
  explicit Or(Constraint&& left, Constraint&& right);
  ~Or();

  Or* Clone() const override;

  bool Validate(const StringAttributeList& attr_map) const override;
  std::string GetRepresentation() const override;
private:
  Constraint m_left;
  Constraint m_right;
};

/**
 * Composite constraint that checks if both the left and the right constraint is satisfied.
 * This is equivalent to an 'AND' operation.
*/
class And : public IConstraint
{
public:
  explicit And(Constraint&& left, Constraint&& right);
  ~And();

  And* Clone() const override;

  bool Validate(const StringAttributeList& attr_map) const override;
  std::string GetRepresentation() const override;
private:
  Constraint m_left;
  Constraint m_right;
};

/**
 * Decorator constraint that checks inverts the validation result of its child contraint.
 * This is equivalent to an 'NOT' operation.
*/
class Not : public IConstraint
{
public:
  explicit Not(Constraint&& child);
  ~Not();

  Not* Clone() const override;

  bool Validate(const StringAttributeList& attr_map) const override;
  std::string GetRepresentation() const override;
private:
  Constraint m_child;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_CONCRETE_CONSTRAINTS_H_
