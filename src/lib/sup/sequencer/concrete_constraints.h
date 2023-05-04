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

#ifndef SUP_SEQUENCER_CONCRETE_CONSTRAINTS_H_
#define SUP_SEQUENCER_CONCRETE_CONSTRAINTS_H_

#include <sup/sequencer/constraint.h>

namespace sup
{
namespace sequencer
{

class Exists : public IConstraint
{
public:
  explicit Exists(const std::string& attr_name);
  ~Exists();

  bool Validate(const ValueMap& attr_map) const override;
  std::string GetRepresentation() const override;
private:
  const std::string m_attr_name;
};

class Either : public IConstraint
{
public:
  explicit Either(Constraint&& left, Constraint&& right);
  ~Either();

  bool Validate(const ValueMap& attr_map) const override;
  std::string GetRepresentation() const override;
private:
  Constraint m_left;
  Constraint m_right;
};

class Both : public IConstraint
{
public:
  explicit Both(Constraint&& left, Constraint&& right);
  ~Both();

  bool Validate(const ValueMap& attr_map) const override;
  std::string GetRepresentation() const override;
private:
  Constraint m_left;
  Constraint m_right;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_CONCRETE_CONSTRAINTS_H_
