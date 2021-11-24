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
 * Copyright (c) : 2010-2020 ITER Organization,
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
 * @file Variable.h
 * @brief Header file for Variable interface.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the Variable interface.
 */

#ifndef _SEQ_Variable_h_
#define _SEQ_Variable_h_

// Global header files

#include <common/AnyValue.h>

#include <condition_variable>
#include <functional>
#include <mutex>

// Local header files

#include "AttributeMap.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
/**
 * @brief Interface for workspace variables.
 */

class Variable
{
private:
  /**
   * @brief Typename of this variable
   */
  const std::string _type;

  /**
   * @brief Mutex for concurrent access of Variable.
   * @details This mutex protects access to all other private member data, unless explicitly
   * mentioned otherwise.
   */
  mutable std::mutex _access_mutex;

  /**
   * @brief List of attributes.
   */
  AttributeMap _attributes;

  /**
   * @brief Indicates if the Variable was correctly setup from its attributes.
   */
  bool _setup_successful;

  /**
   * @brief Mutex for concurrent access of the update counter.
   * @details This mutex protects only access to the update counter.
   */
  mutable std::mutex notify_mutex;

  /**
   * @brief Used to track updates of the underlying value.
   * @details This condition variable is notified after an update.
   */
  mutable std::condition_variable _update_cond;

  /**
   * @brief Callback function to call when value was updated.
   *
   * @note There is currently no option to set multiple callback functions.
   * This is on purpose, while the workspace will register itself here and can provide more
   * complex use cases. This callback is typically called while holding locks on the variable, so
   * it's the responsibility of the listener to prevent deadlock (e.g. by pushing the value to a
   * queue and processing it after returning from the callback).
   */
  std::function<void(const ccs::types::AnyValue&)> notify_cb;

  /**
   * @brief Get value of variable.
   *
   * @param value variable reference to contain the value.
   * @return true on success.
   *
   * @note Private virtual implementation.
   */
  virtual bool GetValueImpl(::ccs::types::AnyValue& value) const = 0;

  /**
   * @brief Set value of variable.
   *
   * @param value value to set.
   * @return true on success.
   *
   * @note Private virtual implementation.
   */
  virtual bool SetValueImpl(const ::ccs::types::AnyValue& value) = 0;

  /**
   * @brief Setup value of variable.
   *
   * @note Private virtual implementation.
   */
  virtual bool SetupImpl();

protected:
public:
  /**
   * @brief Constructor.
   */
  Variable(const std::string& type);

  /**
   * @brief Destructor.
   */
  virtual ~Variable();

  /**
   * @brief Get variable type
   * @return variable type
   */
  std::string GetType() const;

  /**
   * @brief Get variable name
   * @return variable name
   */
  std::string GetName() const;

  /**
   * @brief Set variable name
   * @param name Name to set
   * @return void
   */
  void SetName(const std::string& name);

  /**
   * @brief Get value of variable.
   *
   * @param value variable reference to contain the value.
   * @return true on success.
   *
   * @note Non-virtual interface.
   */
  bool GetValue(::ccs::types::AnyValue& value, const std::string& fieldname = {}) const;

  /**
   * @brief Set value of variable.
   *
   * @param value value to set.
   * @return true on success.
   *
   * @note Non-virtual interface.
   */
  bool SetValue(const ::ccs::types::AnyValue& value, const std::string& fieldname = {});

  /**
   * @brief Notify waiting threads of an update to the variable.
   * @note Needs to be called whenever the variable is updated with a different mechanism as
   * SetValue. It has to be called without holding the mutex lock.
   */
  void Notify(const ccs::types::AnyValue& value);

  /**
   * @brief Set callback for value update notifications
   *
   * @param func Callback function object.
   * @return true if successful.
   *
   * @note This method will overwrite an existing callback if there was one.
   */
  void SetNotifyCallback(std::function<void(const ccs::types::AnyValue&)> func);

  /**
   * @brief Indicate presence of attribute with given name.
   *
   * @param name Attribute name.
   * @return true when present.
   */
  bool HasAttribute(const std::string& name) const;

  /**
   * @brief Get attribute with given name.
   *
   * @param name Attribute name.
   * @return Attribute value.
   */
  std::string GetAttribute(const std::string& name) const;

  /**
   * @brief Set attribute with given name and value.
   *
   * @param name Attribute name.
   * @param value Attribute value.
   * @return true when successful.
   */
  bool AddAttribute(const std::string& name, const std::string& value);

  /**
   * @brief Set all attributes in given list.
   *
   * @param attributes Attribute list.
   * @return true when successful.
   */
  bool AddAttributes(const AttributeMap& attributes);
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

#endif  // _SEQ_Variable_h_
