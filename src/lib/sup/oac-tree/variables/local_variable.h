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

#ifndef SUP_OAC_TREE_LOCAL_VARIABLE_H_
#define SUP_OAC_TREE_LOCAL_VARIABLE_H_

#include <sup/oac-tree/variable.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Variable that is hosted locally in the workspace.
 *
 * @details A LocalVariable with only a type attribute (no value attribute) will
 * allocate a zero-initialized AnyValue with the given type.
 */
class LocalVariable : public Variable
{
public:
  LocalVariable();

  ~LocalVariable() override;

  /**
   * @brief Class name for VariableRegistry.
   */
  static const std::string Type;
private:
  /**
   * @brief Encapsulated AnyValue.
   */
  sup::dto::AnyValue m_value;

  bool IsDynamicallyTyped() const;

  /**
   * @brief See sup::oac_tree::Variable.
   */
  bool GetValueImpl(sup::dto::AnyValue& value) const override;
  bool SetValueImpl(const sup::dto::AnyValue& value) override;
  SetupTeardownActions SetupImpl(const Workspace& ws) override;
  void TeardownImpl() override;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_LOCAL_VARIABLE_H_
