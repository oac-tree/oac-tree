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

namespace sup
{
namespace sequencer
{
using ValueMap = std::map<std::string, sup::dto::AnyValue>;

class IConstraint
{
public:
  virtual ~IConstraint() = default;

  virtual bool Validate(const ValueMap& attr_map) const = 0;

  virtual std::string GetRepresentation() const = 0;
};

/**
 * @brief Class that encapsulates a constraint on a map from string to AnyValue objects.
 */
class Constraint
{
public:
  template <typename T, typename... Args>
  Constraint(Args&&... args);
  ~Constraint();

  bool Validate(const ValueMap& attr_map) const;

  std::string GetRepresentation() const;

private:
  std::unique_ptr<IConstraint> m_impl;
};

template <typename T, typename... Args>
Constraint::Constraint(Args&&... args)
  : m_impl{new T{std::forward<Args>(args)...}}
{}

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_CONSTRAINT_H_
