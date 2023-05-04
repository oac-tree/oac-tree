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

#include <sup/sequencer/attribute_constraint.h>

namespace sup
{
namespace sequencer
{

AttributeConstraint::AttributeConstraint(ConstraintFunction func)
  : m_func{std::move(func)}
{}

AttributeConstraint::~AttributeConstraint() = default;

bool AttributeConstraint::Validate(const AttributeValueMap& attr_map,
                                   const AppendErrorStringFunction& err_func) const
{
  if (!m_func)
  {
    if (err_func)
    {
      err_func(kNoConstraintFunctionError);
    }
    return false;
  }
  auto result = m_func(attr_map);
  if (!result.first && err_func)
  {
    err_func(result.second);
  }
  return result.first;
}

void IgnoreConstraintError(const std::string& err_str)
{
  (void)err_str;
}

}  // namespace sequencer

}  // namespace sup
