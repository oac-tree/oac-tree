/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
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

#ifndef SUP_SEQUENCER_CONSTRAINT_H_
#define SUP_SEQUENCER_CONSTRAINT_H_

#include <sup/dto/anyvalue.h>

#include <map>
#include <memory>
#include <string>
#include <utility>

namespace sup
{
namespace sequencer
{
using StringAttribute = std::pair<std::string, std::string>;
using StringAttributeList = std::vector<StringAttribute>;

const std::string kConstraintEmpty = "Empty constraint (always fails)";

class IConstraint
{
public:
  virtual ~IConstraint() = default;

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

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_CONSTRAINT_H_
