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

#ifndef SUP_SEQUENCER_VARIABLE_H_
#define SUP_SEQUENCER_VARIABLE_H_

#include <sup/sequencer/attribute_handler.h>
#include <sup/sequencer/setup_teardown_actions.h>

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
 *
 * @details Most of the Variable API is implemented using the non-virtual interface (NVI) idiom and
 * locks are provided for those public methods that can be called during execution to ensure
 * thread safety. Those methods are:
 *   - Value access functions: GetValue, SetValue and IsAvailable;
 *   - Notification callback functions: Notify and SetNotifyCallback.
 * Note that all other methods are not thread safe and thus should be called only from a single
 * thread. These methods are typically called during the initialization of a procedure.
 */
class Variable
{
public:
  using Callback = std::function<void(const sup::dto::AnyValue&, bool)>;

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
   * @param ws Current workspace.
   * @return Optional setup/teardown actions that need to be called once per class of Variables
   * (identified by a unique string identifier).
   *
   * @throw VariableSetupException when the variable could not be setup properly.
   */
  SetupTeardownActions Setup(const Workspace& ws);

  /**
   * @brief Resets the variable to the state just after the initial Setup.
   *
   * @details By default, this methods calls Teardown and Setup, but custom variables may override
   * this behavior.
   *
   * @param ws Current workspace.
   *
   * @throw VariableSetupException when the variable could not be reset properly.
   */
  void Reset(const Workspace& ws);

  /**
   * @brief Get value of variable.
   *
   * @param value variable reference to contain the value.
   * @param fieldname optional field name.
   * @return true on success.
   *
   * @note Non-virtual interface. This member function will fail, i.e. return false, if value is
   * non-empty and the value to be retrieved cannot be converted to value's type.
   */
  bool GetValue(sup::dto::AnyValue& value, const std::string& fieldname = {}) const;

  /**
   * @brief Set value of variable.
   *
   * @param value value to set.
   * @param fieldname optional field name.
   * @return true on success.
   *
   * @note Non-virtual interface. This member function does not enforce any type constraints on
   * the value. Variable implementations are fully responsible to enforce this if needed.
   */
  bool SetValue(const sup::dto::AnyValue& value, const std::string& fieldname = {});

  /**
   * @brief Check if variable is available.
   *
   * @return true on success.
   *
   * @note Non-virtual interface.
   * @note Availability is context-dependent: e.g. a network variable is available when it is
   * connected and its value can be read.
   */
  bool IsAvailable() const;

  /**
   * @brief Notify waiting threads of an update to the variable.
   *
   * @param value New value of variable.
   * @param connected New connectivity status of variable.
   *
   * @note Needs to be called whenever the variable is updated. It has to be called without
   * holding the mutex lock.
   */
  void Notify(const sup::dto::AnyValue& value, bool connected) const;

  /**
   * @brief Set callback for value update notifications.
   *
   * @param func Callback function object.
   * @return true if successful.
   *
   * @note This method will overwrite an existing callback if there was one.
   */
  void SetNotifyCallback(Callback func);

  /**
   * @brief Tear down the variable.
   * @details This method resets the variable to its initial, i.e. uninitialized, state. For
   * example, network variables will disconnect.
   */
  void Teardown();

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
   * @param attr_name Attribute name.
   * @return Attribute value.
   */
  std::string GetAttributeString(const std::string& attr_name) const;

  /**
   * @brief Get all attributes.
   *
   * @return List containing all attributes.
   */
  const StringAttributeList& GetStringAttributes() const;

  /**
   * @brief Set attribute with given name and value.
   *
   * @param attr_name Attribute name.
   * @param attr_value Attribute value.
   * @return true when successful.
   */
  bool AddAttribute(const std::string& attr_name, const std::string& attr_value);

  /**
   * @brief Set all attributes in given list.
   *
   * @param str_attributes Attribute list.
   * @return true when successful.
   */
  bool AddAttributes(const StringAttributeList& str_attributes);

  /**
   * @brief Get attribute value with given name and type.
   *
   * @param attr_name Attribute name.
   * @param val Output parameter for value.
   *
   * @return True on success or when the attribute is not present.
   *
   * @note The reason for returning true in the absence of the attribute is that mandatory
   * attributes are already checked during setup and non-mandatory attributes should not cause an
   * error when they are not present.
   */
  template <typename T>
  bool GetAttributeValue(const std::string& attr_name, T& val) const
  {
    if (!HasAttribute(attr_name))
    {
      return true;
    }
    return m_attribute_handler.GetValueAs(attr_name, val);
  }

  /**
   * @brief Get all attribute definitions.
   *
   * @return List of attribute definitions.
   */
  const std::vector<AttributeDefinition>& GetAttributeDefinitions() const;

protected:
  /**
   * @brief Add an attribute definition with the given name and type.
   *
   * @param attr_name Attribute name.
   * @param value_type Attribute type.
   * @return Newly added attribute definition.
   */
  AttributeDefinition& AddAttributeDefinition(const std::string& attr_name,
                                              const sup::dto::AnyType& value_type);

  /**
   * @brief Add an attribute constraint.
   *
   * @param constraint Attribute constraint.
   */
  void AddConstraint(Constraint constraint);

private:
  /**
   * @brief Typename of this variable
   */
  const std::string m_type;

  /**
   * @brief Attribute handler.
  */
  AttributeHandler m_attribute_handler;

  /**
   * @brief Indicates if the Variable was correctly setup from its attributes.
   */
  bool m_setup_successful;

  /**
   * @brief Mutex for concurrent access of Variable.
   * @details This mutex protects access to all other private member data, unless explicitly
   * mentioned otherwise.
   */
  mutable std::mutex m_access_mutex;

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
  Callback m_notify_cb;

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
   * @param ws Parent workspace.
   * @return Optional setup/teardown actions that need to be called once per class of Variables
   * (identified by a unique string identifier).
   *
   * @throw VariableSetupException when the variable could not be setup properly.
   *
   * @note Private virtual implementation.
   */
  virtual SetupTeardownActions SetupImpl(const Workspace& ws);

  /**
   * @brief Resets the variable to the state just after the initial Setup.
   *
   * @param ws Parent workspace.
   *
   * @throw VariableSetupException when the variable could not be reset properly.
   *
   * @note Private virtual implementation.
   */
  virtual void ResetImpl(const Workspace& ws);

  /**
   * @brief Tear down the variable.
   *
   * @note Private virtual implementation.
   */
  virtual void TeardownImpl();
};

/**
 * @brief Construct a string prolog for throwing exceptions related to Variable::Setup.
 *
 * @param variable Variable to use.
 *
 * @return String containing the prolog to be used for the exception message.
 */
std::string VariableSetupExceptionProlog(const Variable& variable);

/**
 * @brief Construct an anyvalue from a set of attributes holding the json type and value
 * representation.
 *
 * @param variable Variable to use.
 * @param type_attr_name Attribute name for the json type.
 * @param value_attr_name Attribute name for the json value.
 * @param registry Type registry to use for parsing the type.
 *
 * @return AnyValue created from parsing the set of attributes. If the json type attribute is not
 * present, returns empty. If the json value attribute is not present, returns a default constructed
 * value from the type.
 * @throws VariableSetupException when the parsing fails.
 */
sup::dto::AnyValue ParseAnyValueAttributePair(const Variable& variable,
                                              const std::string& type_attr_name,
                                              const std::string& value_attr_name,
                                              const sup::dto::AnyTypeRegistry& registry);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_VARIABLE_H_
