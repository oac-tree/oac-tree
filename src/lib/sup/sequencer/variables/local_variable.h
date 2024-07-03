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
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_LOCAL_VARIABLE_H_
#define SUP_SEQUENCER_LOCAL_VARIABLE_H_

#include <sup/sequencer/variable.h>

namespace sup
{
namespace sequencer
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
   * @brief See sup::sequencer::Variable.
   */
  bool GetValueImpl(sup::dto::AnyValue& value) const override;
  bool SetValueImpl(const sup::dto::AnyValue& value) override;
  SetupTeardownActions SetupImpl(const Workspace& ws) override;
  void TeardownImpl() override;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_LOCAL_VARIABLE_H_
