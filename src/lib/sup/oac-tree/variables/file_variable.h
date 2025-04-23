/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) oac-tree component
 *
 * Description   : File-based variable implementation
 *
 * Author        : Bertrand Bauvir (IO)
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

#ifndef SUP_OAC_TREE_FILE_VARIABLE_H_
#define SUP_OAC_TREE_FILE_VARIABLE_H_

#include <sup/oac-tree/variable.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief FileVariable class.
 * @details Variable with file-based backend.
 */
class FileVariable : public Variable
{
public:
  FileVariable();
  ~FileVariable() override;

  /**
   * @brief Class name for VariableRegistry.
   */
  static const std::string Type;

private:
  std::string m_workspace_path;
  /**
   * @brief See sup::oac_tree::Variable.
   */
  bool GetValueImpl(sup::dto::AnyValue& value) const override;
  bool SetValueImpl(const sup::dto::AnyValue& value) override;
  SetupTeardownActions SetupImpl(const Workspace& ws) override;
  bool IsAvailableImpl() const override;

  /**
   * @brief Try to read the value from the file and return empty unique_ptr on failure.
   *
   * @return unique_ptr to read value or empty one on failure.
   */
  std::unique_ptr<sup::dto::AnyValue> ReadValue() const;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_FILE_VARIABLE_H_
