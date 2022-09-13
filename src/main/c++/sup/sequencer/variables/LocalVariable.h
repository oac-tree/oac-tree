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
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

/**
 * @file LocalVariable.h
 * @brief Header file for LocalVariable class.
 * @date 11/12/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the LocalVariable class.
 */

#ifndef _SEQ_LocalVariable_h_
#define _SEQ_LocalVariable_h_

// Global header files

#include <memory>

// Local header files

#include <sup/sequencer/variable.h>

// Constants

// Type definition

#ifdef __cplusplus

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
  std::unique_ptr<::ccs::types::AnyValue> val;

  /**
   * @brief See sup::sequencer::Variable.
   */
  bool GetValueImpl(::ccs::types::AnyValue& value) const override;
  bool SetValueImpl(const ::ccs::types::AnyValue& value) override;
  bool SetupImpl() override;
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

// Global variables

// Function declarations

// Function definitions

}  // namespace sequencer

}  // namespace sup

extern "C"
{
#endif  // __cplusplus

  // C API function declarations

#ifdef __cplusplus
}  // extern C
#endif  // __cplusplus

#endif  // _SEQ_LocalVariable_h_
