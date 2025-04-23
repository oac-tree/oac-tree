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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_CONSTRAINT_H_
#define SUP_OAC_TREE_CONSTRAINT_H_

#include <sup/dto/anyvalue.h>

#include <map>
#include <memory>
#include <string>
#include <utility>

namespace sup
{
namespace oac_tree
{
using StringAttribute = std::pair<std::string, std::string>;
using StringAttributeList = std::vector<StringAttribute>;

const std::string kConstraintEmpty = "Empty constraint (always fails)";

class IConstraint
{
public:
  virtual ~IConstraint();

  virtual IConstraint* Clone() const = 0;

  virtual bool Validate(const StringAttributeList& attr_map) const = 0;

  virtual std::string GetRepresentation() const = 0;
};

/**
 * @brief Class that encapsulates a constraint on a map from string to AnyValue objects.
 */
class Constraint
{
public:
  ~Constraint();
  Constraint(std::unique_ptr<IConstraint>&& impl);

  // Copy/move
  Constraint(const Constraint& other);
  Constraint(Constraint&& other);
  Constraint& operator=(const Constraint& other);
  Constraint& operator=(Constraint&& other);

  bool Validate(const StringAttributeList& attr_map) const;

  std::string GetRepresentation() const;

private:
  std::unique_ptr<IConstraint> m_impl;
};

template <typename T, typename... Args>
Constraint MakeConstraint(Args&&... args)
{
  return Constraint{std::unique_ptr<IConstraint>{new T{std::forward<Args>(args)...}}};
}

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_CONSTRAINT_H_
