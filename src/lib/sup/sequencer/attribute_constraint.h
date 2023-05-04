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

#ifndef SUP_SEQUENCER_ATTRIBUTE_CONSTRAINT_H_
#define SUP_SEQUENCER_ATTRIBUTE_CONSTRAINT_H_

#include <sup/dto/anyvalue.h>

#include <functional>
#include <map>
#include <string>
#include <utility>

namespace sup
{
namespace sequencer
{
using AttributeValueMap = std::map<std::string, sup::dto::AnyValue>;
using ConstraintFunction =
  std::function<std::pair<bool, std::string>(const AttributeValueMap&)>;
using AppendErrorStringFunction = std::function<void(const std::string&)>;

const std::string kNoConstraintFunctionError =
  "Attribute constrained does not contain a valid constraint function";

/**
 * @brief Class that encapsulates a constraint on a set of attributes.
 */
class AttributeConstraint
{
public:
  AttributeConstraint(ConstraintFunction func);
  ~AttributeConstraint();

  bool Validate(const AttributeValueMap& attr_map, const AppendErrorStringFunction& err_func) const;

private:
  ConstraintFunction m_func;
};

/**
 * Function that can be passed to ignore error strings.
*/
void IgnoreConstraintError(const std::string& err_str);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_ATTRIBUTE_CONSTRAINT_H_
