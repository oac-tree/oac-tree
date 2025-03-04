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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_INSTRUCTION_H_
#define SUP_OAC_TREE_INSTRUCTION_H_

#include <atomic>
#include <mutex>

#include <sup/oac-tree/attribute_handler.h>
#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/user_interface.h>

#include <sup/dto/anyvalue.h>

namespace sup
{
namespace oac_tree
{
class Procedure;
class UserInterface;
class Workspace;

/**
 * @brief Abstract interface for all executable instructions
 * @details Instruction is the base class for all executable
 * instructions. These include: sequences, selectors, decorator
 * nodes and leaf instruction nodes
 *
 * @note The design of the execution of these instructions
 * is based on private virtual implementation.
 */
class Instruction
{
public:
  /**
   * Enumeration that indicates if the instruction is a simple action (no child instructions),
   * a decorator (one child) or a compound instruction (multiple children).
  */
  enum Category : sup::dto::uint32
  {
    kAction = 0,
    kDecorator,
    kCompound
  };

  /**
   * @brief Constructor.
   * @param type The type of instruction.
   */
  Instruction(const std::string &type);

  /**
   * @brief Virtual destructor.
   */
  virtual ~Instruction();

  /**
   * @brief Get instruction category.
   * @return instruction category.
   */
  virtual Category GetCategory() const;

  /**
   * @brief Get instruction type.
   * @return instruction type.
   */
  std::string GetType() const;

  /**
   * @brief Get instruction name.
   * @return instruction name (or empty string when name was not found).
   */
  std::string GetName() const;

  /**
   * @brief Set instruction name.
   * @param name Name to set.
   */
  void SetName(const std::string& name);

  /**
   * @brief Get execution status.
   * @return Execution status.
   */
  ExecutionStatus GetStatus() const;

  /**
   * @brief Setup method.
   *
   * @param proc Procedure containing Workspace and instruction declarations.
   * @throw InstructionSetupException when the instruction could not be setup properly.
   */
  void Setup(const Procedure& proc);

  /**
   * @brief Execution method.
   * @param ui UserInterface to handle input/output.
   * @param ws Workspace that contains the variables.
   */
  void ExecuteSingle(UserInterface& ui, Workspace& ws);

  /**
   * @brief Halt execution.
   * @details Only meaningful when the instruction is running asynchronously.
   * This method only sets an atomic boolean member variable. It is up to implementations
   * of the Instruction to check this variable regularly to prevent long blocking. It is
   * expected that an instruction will fail if it is interrupted (status: FAILURE).
   */
  void Halt();

  /**
   * @brief Reset execution status, so the instruction can be executed again with initial conditions.
   *
   * @param ui UserInterface to handle status updates.
   *
   * @details The state of the instruction after a reset should be similar to its state after an
   * initial Setup call.
   * This method call blocks until the termination of all descendant instructions
   * running in a separate thread. Destruction of this instruction is safe afterwards.
   * @note This method should only be called on instruction objects that are not currently
   * executing. This is taken care of by waiting for child termination before calling their
   * Instruction::Reset() method.
   */
  void Reset(UserInterface& ui);

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
  std::string GetAttributeString(const std::string& name) const;

  /**
   * @brief Get all attributes.
   *
   * @return List containing all attributes.
   */
  const StringAttributeList& GetStringAttributes() const;

  /**
   * @brief Set new attribute with given name and value.
   *
   * @param name Attribute name.
   * @param value Attribute value.
   * @return true when successful.
   */
  bool AddAttribute(const std::string& name, const std::string& value);

  /**
   * @brief Set attribute with given name and value.
   *
   * @param name Attribute name.
   * @param value Attribute value.
   * @return true when successful.
   */
  bool SetAttribute(const std::string& name, const std::string& value);

  /**
   * @brief Set all attributes from given map.
   *
   * @param attributes List of attributes to set.
   * @return true when successful.
   */
  bool AddAttributes(const StringAttributeList& attributes);

  /**
   * @brief Initialise variable attributes with values from attribute map.
   *
   * @param source_attributes List containing variable name - value pairs.
   * @return true when all variable attributes were correctly initialised.
   */
  bool InitialisePlaceholderAttributes(const StringAttributeList& source_attributes);

  /**
   * @brief Get an AnyValue representation of an attribute. This function handles attributes that
   * encode literal values, values that need to be fetched from a workspace variable or both,
   * depending on the attribute's definition.
   *
   * @param attr_name Attribute name.
   * @param ws Workspace to use when the value needs to be fetched.
   * @param ui UserInterface to use for logging errors or warnings.
   * @param value Output value when successful.
   *
   * @return True on success or when the attribute is not present.
   *
   * @note The reason for returning true in the absence of the attribute is that mandatory
   * attributes are already checked during setup and non-mandatory attributes should not cause an
   * error when they are not present.
   */
  bool GetAttributeValue(const std::string& attr_name, const Workspace& ws,
                         UserInterface& ui, sup::dto::AnyValue& value) const;

  /**
   * @brief Get a representation of type T of an attribute's value. This function handles attributes
   * that encode literal values, values that need to be fetched from a workspace variable or both,
   * depending on the attribute's definition.
   *
   * @param attr_name Attribute name.
   * @param ws Workspace to use when the value needs to be fetched.
   * @param ui UserInterface to use for logging errors or warnings.
   * @param val Output value when successful.
   *
   * @return True on success or when the attribute is not present.
   *
   * @note The reason for returning true in the absence of the attribute is that mandatory
   * attributes are already checked during setup and non-mandatory attributes should not cause an
   * error when they are not present.
   */
  template <typename T>
  bool GetAttributeValueAs(const std::string& attr_name, const Workspace& ws,
                           UserInterface& ui, T& val) const;

  /**
   * @brief Get all attribute definitions.
   *
   * @return List of attribute definitions.
   */
  const std::vector<AttributeDefinition>& GetAttributeDefinitions() const;

  /**
   * @brief Returns children count.
   *
   * @return children count
   */
  int ChildrenCount() const;

  /**
   * @brief Get list of child instructions.
   *
   * @return List of child instructions.
   */
  std::vector<Instruction*> ChildInstructions();

  /**
   * @brief Get list of child instructions (const version).
   *
   * @return List of child instructions.
   */
  std::vector<const Instruction*> ChildInstructions() const;

  /**
   * @brief Get list of child instructions that will be executed in the next execute step.
   *
   * @return List of child instructions.
   *
   * @details This list of child instructions is only scheduled to be executed next if the current
   * instruction also is, i.e. it apprears in the NextInstructions() list of the parent or is a
   * root instruction that is NOT_STARTED/NOT_FINISHED.
   */
  std::vector<const Instruction*> NextInstructions() const;

  /**
   * @brief Inserts child into the given index.
   *
   * @return true on success
   * @details Index must be in the range [0, current_children_count] inclusively.
   */
  bool InsertInstruction(std::unique_ptr<Instruction>&& child, int index);

  /**
   * @brief Removes child with the given index and returns it to the user.
   *
   * @return child on success, nullptr otherwise
   * @details Index must be in the range [0, current_children_count-1].
   */
  std::unique_ptr<Instruction> TakeInstruction(int index);

  /**
   * @brief Check if a request to halt this instruction was issued.
   */
  bool IsHaltRequested() const;

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
   * @brief Adds an attribute definition with the given name and default type: StringType
   *
   * @param attr_name Attribute name.
   * @return Newly added attribute definition.
  */
  AttributeDefinition& AddAttributeDefinition(const std::string& attr_name);

  /**
   * @brief Add an attribute constraint.
   *
   * @param constraint Attribute constraint.
   */
  void AddConstraint(Constraint constraint);

private:
  /**
   * @brief Typename of this instruction
   */
  const std::string m_type;

  ExecutionStatus m_status;

  ExecutionStatus m_status_before;
  /**
   * @brief Atomic flag that indicates if a request was made to halt the instruction.
   */
  std::atomic_bool m_halt_requested;

  /**
   * @brief Attribute handler.
  */
 AttributeHandler m_attribute_handler;

  /**
   * @brief Mutex for thread safe access to the Instruction's execution status.
   *
   * @note Only locked for Get/SetStatus to prevent deadlock.
   */
  mutable std::mutex m_status_mutex;

  /**
   * @brief Set the Instruction's execution status.
   *
   * @note Provided for thread safety.
   */
  void SetStatus(ExecutionStatus status);

  /**
   * @brief Private method that is always called before delegating further execution to the
   * private virtual Instruction::ExecuteSingleImpl(UserInterface * ui, Workspace * ws).
   *
   * @details If the instruction was not yet executed it calls Instruction::InitHook,
   * changes the status to ExecutionStatus::NOT_FINISHED and informs the user interface of a
   * status change.
   */
  void Preamble(UserInterface& ui, Workspace& ws);

  /**
   * @brief Private method that is always called the delegated execution to the
   * private virutal Instruction::ExecuteSingleImple(UserInterface * ui, Workspace * ws).
   *
   * @details This method first checks if the instruction's status was changed as a consequence
   * of the call to Instruction::ExecuteSingleImple(UserInterface * ui, Workspace * ws). If so,
   * it informs the user interface of this status change.
   */
  void Postamble(UserInterface& ui);

  /**
   * @brief Setup implementation.
   *
   * @param proc Procedure containing Workspace and instruction declarations.
   *
   * @throw InstructionSetupException when the instruction could not be setup properly.
   *
   * @details Successful completion of this instruction means that the instruction
   * was able to configure itself from its attributes and (possibly) data from the Procedure.
   * This method is called from the Instruction::Setup(const Procedure & proc) method.
   */
  virtual void SetupImpl(const Procedure& proc);

  /**
   * @brief Private hook that is called at the start of the first ExecuteSingle call.
   *
   * @return true on successful initialization.
   *
   * @details This hook is meant to establish a correct starting state (e.g. reset internal
   * counters) and will be called every time the instruction is executed afresh, i.e. restarted
   * from scratch. See Instruction::SetupImpl(const Procedure & proc) for establishing correct
   * configuration state of the instruction.
   * @note Default implementation is empty.
   */
  virtual bool InitHook(UserInterface& ui, Workspace& ws);

  /**
   * @brief Private implementation of the execute action.
   *
   * @details Pure virtual: this method contains the action(s) to be taken during execution.
   */
  virtual ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) = 0;

  /**
   * @brief Hook called during Instruction::Halt().
   *
   * @details This hook is meant to propagate the call to Instruction::Halt() to
   * the instructions's child instructions.
   */
  virtual void HaltImpl();

  /**
   * @brief Private hook that is always called during a call to Instruction::Reset().
   *
   * @details This hook can be used to ensure that all descendant child instructions running
   * in a separate thread have finished execution. It is also used to propagate the call to
   * Instruction::Reset() to all child instructions.
   * @note Default implementation is empty.
   */
  virtual void ResetHook(UserInterface& ui);

  /**
   * @brief Private hook that is called after variable initialisation.
   *
   * @return true on succes.
   * @details Mainly used to propagate attributes to nested instructions.
   * Default implementation returns true.
   */
  virtual bool PostInitialiseVariables(const StringAttributeList& source_attributes);

  /**
   * @brief Returns children count.
   *
   * @return children count
   */
  virtual int ChildrenCountImpl() const;

  /**
   * @brief Get list of child instructions implementation (const version).
   *
   * @return List of child instructions.
   */
  virtual std::vector<const Instruction*> ChildInstructionsImpl() const;

  /**
   * @brief Get list of child instructions that will be executed in the next execute step.
   *
   * @return List of child instructions.
   */
  virtual std::vector<const Instruction*> NextInstructionsImpl() const;

  /**
   * @brief Inserts child into the given index.
   *
   * @return true on success
   * @details Index must be in the range [0, current_children_count] inclusively.
   */
  virtual bool InsertInstructionImpl(std::unique_ptr<Instruction>&& child, int index);

  /**
   * @brief Removes child with the given index and returns it to the user.
   *
   * @return child on success, nullptr otherwise
   * @details Index must be in the range [0, current_children_count-1].
   */
  virtual std::unique_ptr<Instruction> TakeInstructionImpl(int index);
};

/**
 * @brief Append a child instruction to an instruction if that is supported.
 *
 * @param instruction Instruction to add child to.
 * @param child Instruction to add.
 *
 * @return True if the adding was successful.
 */
bool AppendChildInstruction(Instruction& instruction, std::unique_ptr<Instruction>&& child);

/**
 * @brief Construct a string prolog for throwing exceptions related to Instruction::Setup.
 *
 * @param instruction Instruction to use.
 *
 * @return String containing the prolog to be used for the exception message.
 */
std::string InstructionSetupExceptionProlog(const Instruction& instruction);

/**
 * @brief Construct a string prolog for reporting errors to the UserInterface.
 *
 * @param instruction Instruction to use.
 *
 * @return String containing the prolog to be used for the error message.
 */
std::string InstructionErrorProlog(const Instruction& instruction);

/**
 * @brief Construct a string prolog for reporting warnings to the UserInterface.
 *
 * @param instruction Instruction to use.
 *
 * @return String containing the prolog to be used for the warning message.
 */
std::string InstructionWarningProlog(const Instruction& instruction);

// Definition needs to come after declaration of InstructionWarningProlog
template <typename T>
bool Instruction::GetAttributeValueAs(const std::string& attr_name, const Workspace& ws,
                                      UserInterface& ui, T& val) const
{
  if (!HasAttribute(attr_name))
  {
    return true;
  }
  sup::dto::AnyValue temp;
  if (!GetAttributeValue(attr_name, ws, ui, temp))
  {
    return false;
  }
  if (!temp.As(val))
  {
    std::string warning_message =
      InstructionWarningProlog(*this) + "could not convert attribute with name ["
      + attr_name + "] to the expected type";
    LogWarning(ui, warning_message);
    return false;
  }
  return true;
}

/**
 * @brief Set variable (and field) with name contained in the instruction's attribute.
 *
 * @param instruction Instruction containing the attribute.
 * @param ws Workspace containing the variables
 * @param ui UserInterface to use to report errors or warnings.
 * @param attr_name Attribute name.
 * @param value AnyValue that will be copied to the workspace's variable.
 *
 * @return True if successful.
 */
bool SetValueFromAttributeName(const Instruction& instruction, Workspace& ws,
                               UserInterface& ui, const std::string& attr_name,
                               const sup::dto::AnyValue& value);

/**
 * @brief Construct an anyvalue from a pair of attributes holding the json type and value
 * representation.
 *
 * @param instruction Instruction to use.
 * @param ws Workspace that can hold type definitions.
 * @param ui UserInterface to log errors to.
 * @param type_attr_name Attribute name for the json type.
 * @param value_attr_name Attribute name for the json value.
 *
 * @return AnyValue created from parsing the set of attributes. Both type and value attributes
 * need to exist and be correctly formatted to parse them.
 */
sup::dto::AnyValue ParseAnyValueAttributePair(const Instruction& instruction,
                                              const Workspace& ws,
                                              UserInterface& ui,
                                              const std::string& type_attr_name,
                                              const std::string& value_attr_name);

/**
 * @brief Query all attribute names that are predefined for the given instruction.
 *
 * @param instruction Instruction to query.
 *
 * @return List of attribute names.
 */
std::vector<std::string> GetDefinedAttributeNames(const Instruction& instruction);

/**
 * @brief Query if an attribute name is among the predefined ones of the given instruction.
 *
 * @param instruction Instruction to query.
 * @param attr_name Attribute name to check.
 *
 * @return True if the given name is among the predefined attribute names of the given
 * instruction.
 */
bool IsDefinedAttributeName(const Instruction& instruction, const std::string& attr_name);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_INSTRUCTION_H_
