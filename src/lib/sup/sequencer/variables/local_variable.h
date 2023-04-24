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

#ifndef SUP_SEQUENCER_LOCAL_VARIABLE_H_
#define SUP_SEQUENCER_LOCAL_VARIABLE_H_

#include <sup/sequencer/variable.h>

#include <memory>

namespace sup
{
namespace sequencer
{
/**
 * @brief Variable that is hosted locally in the workspace.
 *
 * @details A LocalVariable with only a type attribute (no value attribute) will
 * allocate an AnyValue, but subsequent GetValue calls will return false until an
 * explicit successful SetValue occurs. The rationale is that it does not make sense to
 * query an zero-initialized value and that this allows for triggering instruction
 * execution on availability of a workspace variable.
 */
class LocalVariable : public Variable
{
private:
  /**
   * @brief Encapsulated AnyValue.
   */
  std::unique_ptr<sup::dto::AnyValue> m_value;

  /**
   * @brief See sup::sequencer::Variable.
   */
  bool GetValueImpl(sup::dto::AnyValue& value) const override;
  bool SetValueImpl(const sup::dto::AnyValue& value) override;
  void SetupImpl(const sup::dto::AnyTypeRegistry& registry) override;
  void ResetImpl() override;

protected:
public:
  /**
   * @brief Constructor.
   */
  LocalVariable();

  /**
   * @brief Destructor.
   */
  ~LocalVariable() override;

  /**
   * @brief Class name for VariableRegistry.
   */
  static const std::string Type;

  /**
   * @brief Defined attribute names.
   */
  static const std::string JSON_TYPE;
  static const std::string JSON_VALUE;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_LOCAL_VARIABLE_H_
