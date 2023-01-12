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

#ifndef SUP_SEQUENCER_VARIABLE_H_
#define SUP_SEQUENCER_VARIABLE_H_

#include <sup/sequencer/attribute_map.h>

#include <sup/dto/anyvalue.h>

#include <condition_variable>
#include <functional>
#include <mutex>

namespace sup
{
namespace dto
{
class AnyTypeRegistry;
}  // namespace dto

namespace sequencer
{
/**
 * @brief Interface for workspace variables.
 */
class Variable
{
public:
  Variable(const std::string& type);

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
   *
   * @param name Name to set
   * @return void
   */
  void SetName(const std::string& name);

  /**
   * @brief Setup variable method.
   *
   * @param registry Type registry.
   * @throw VariableSetupException when the variable could not be setup properly.
   */
  void Setup(const sup::dto::AnyTypeRegistry* registry = nullptr);

  /**
   * @brief Get value of variable.
   *
   * @param value variable reference to contain the value.
   * @return true on success.
   *
   * @note Non-virtual interface.
   */
  bool GetValue(sup::dto::AnyValue& value, const std::string& fieldname = {}) const;

  /**
   * @brief Set value of variable.
   *
   * @param value value to set.
   * @return true on success.
   *
   * @note Non-virtual interface.
   */
  bool SetValue(const sup::dto::AnyValue& value, const std::string& fieldname = {});

  /**
   * @brief Check if variable is available.
   *
   * @return true on success.
   *
   * @note Non-virtual interface.
   * @note Availability is context-dependent: e.g. a network variable is available when it is
   * connected.
   */
  bool IsAvailable() const;

  /**
   * @brief Notify waiting threads of an update to the variable.
   * @note Needs to be called whenever the variable is updated with a different mechanism as
   * SetValue. It has to be called without holding the mutex lock.
   */
  void Notify(const sup::dto::AnyValue& value);

  /**
   * @brief Set callback for value update notifications
   *
   * @param func Callback function object.
   * @return true if successful.
   *
   * @note This method will overwrite an existing callback if there was one.
   */
  void SetNotifyCallback(std::function<void(const sup::dto::AnyValue&)> func);

  /**
   * @brief Reset variable
   * @details This method resets the variable to its initial, i.e. uninitialized, state. For
   * example, network variables will disconnect.
   */
  void Reset();

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
   * @brief Get all attributes.
   *
   * @return Map containing all attributes.
   */
  AttributeMap GetAttributes() const;

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
  bool AddAttributes(const std::vector<Attribute>& attributes);

protected:
  /**
   * Convenience function for generating a prolog for exceptions, containing both the variable's
   * type and object name.
  */
  std::string VariableSetupExceptionProlog() const;

private:
  /**
   * @brief Typename of this variable
   */
  const std::string m_type;

  /**
   * @brief Mutex for concurrent access of Variable.
   * @details This mutex protects access to all other private member data, unless explicitly
   * mentioned otherwise.
   */
  mutable std::mutex m_access_mutex;

  /**
   * @brief List of attributes.
   */
  AttributeMap m_attributes;

  /**
   * @brief Indicates if the Variable was correctly setup from its attributes.
   */
  bool m_setup_successful;

  /**
   * @brief Mutex for concurrent access of the update counter.
   * @details This mutex protects only access to the update counter.
   */
  mutable std::mutex m_notify_mutex;

  /**
   * @brief Used to track updates of the underlying value.
   * @details This condition variable is notified after an update.
   */
  mutable std::condition_variable m_update_cond;

  /**
   * @brief Callback function to call when value was updated.
   *
   * @note There is currently no option to set multiple callback functions.
   * This is on purpose, while the workspace will register itself here and can provide more
   * complex use cases. This callback is typically called while holding locks on the variable, so
   * it's the responsibility of the listener to prevent deadlock (e.g. by pushing the value to a
   * queue and processing it after returning from the callback).
   */
  std::function<void(const sup::dto::AnyValue&)> notify_cb;

  /**
   * @brief Get value of variable.
   *
   * @param value variable reference to contain the value.
   * @return true on success.
   *
   * @note Private virtual implementation.
   */
  virtual bool GetValueImpl(sup::dto::AnyValue& value) const = 0;

  /**
   * @brief Set value of variable.
   *
   * @param value value to set.
   * @return true on success.
   *
   * @note Private virtual implementation.
   */
  virtual bool SetValueImpl(const sup::dto::AnyValue& value) = 0;

  /**
   * @brief Check if variable is available.
   *
   * @return true on success.
   *
   * @note Private virtual implementation.
   * @note Availability is context-dependent: e.g. a network variable is available when it is
   * connected. The default implementation always returns true.
   */
  virtual bool IsAvailableImpl() const;

  /**
   * @brief Setup value of variable.
   *
   * @param registry Type registry.
   *
   * @throw VariableSetupException when the variable could not be setup properly.
   *
   * @note Private virtual implementation.
   */
  virtual void SetupImpl(const sup::dto::AnyTypeRegistry& registry);

  /**
   * @brief Reset variable.
   *
   * @note Private virtual implementation.
   */
  virtual void ResetImpl();
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_VARIABLE_H_
