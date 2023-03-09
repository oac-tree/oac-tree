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

#ifndef SUP_SEQUENCER_INSTRUCTION_H_
#define SUP_SEQUENCER_INSTRUCTION_H_

#include <atomic>
#include <mutex>

#include <sup/sequencer/attribute_map.h>
#include <sup/sequencer/execution_status.h>

namespace sup
{
namespace sequencer
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
   * @brief Constructor.
   * @param type The type of instruction.
   */
  Instruction(const std::string &type);

  /**
   * @brief Virtual destructor.
   */
  virtual ~Instruction();

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
  void SetName(const std::string &name);

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
  void ExecuteSingle(UserInterface* ui, Workspace* ws);

  /**
   * @brief Halt execution.
   * @details Only meaningful when the instruction is running asynchronously.
   * This method only sets an atomic boolean member variable. It is up to implementations
   * of the Instruction to check this variable regularly to prevent long blocking. It is
   * expected that an instruction will fail if it is interrupted (status: FAILURE).
   */
  void Halt();

  /**
   * @brief Reset execution status
   * @details This method call blocks until the termination of all descendant instructions
   * running in a separate thread. Destruction of this instruction is safe afterwards.
   * @note This method should only be called on instruction objects that are not currently
   * executing. This is taken care of by waiting for child termination before calling their
   * Instruction::Reset() method.
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
  bool AddAttributes(const AttributeMap& attributes);

  /**
   * @brief Initialise variable attributes with values from attribute map.
   *
   * @param source Map containing variable name - value pairs.
   * @return true when all variable attributes were initialised.
   */
  bool InitialiseVariableAttributes(const AttributeMap& source);

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
  std::vector<Instruction *> ChildInstructions();

  /**
   * @brief Get list of child instructions (const version).
   *
   * @return List of child instructions.
   */
  std::vector<const Instruction *> ChildInstructions() const;

  /**
   * @brief Inserts child into the given index.
   *
   * @return true on success
   * @details Index must be in the range [0, current_children_count] inclusively.
   */
  bool InsertInstruction(Instruction* child, int index);

  /**
   * @brief Removes child with the given index and returns it to the user.
   *
   * @return child on success, nullptr otherwise
   * @details Index must be in the range [0, current_children_count-1], ownership
   * is given to the user.
   */
  Instruction* TakeInstruction(int index);

protected:
  /**
   * @brief Atomic flag that indicates if a request was made to halt the instruction.
   */
  std::atomic_bool _halt_requested;

  /**
   * Convenience functions for generating prologs for exceptions, errors and warnings,
   * containing both the instruction's type and object name.
  */
  std::string InstructionErrorLogProlog() const;
  std::string InstructionWarningLogProlog() const;

private:
  /**
   * @brief Typename of this instruction
   */
  const std::string _type;

  ExecutionStatus _status;

  ExecutionStatus _status_before;

  AttributeMap _attributes;

  /**
   * @brief Mutex for thread safe access to the Instruction's execution status.
   *
   * @note Only locked for Get/SetStatus to prevent deadlock.
   */
  mutable std::mutex _status_mutex;

  /**
   * @brief Set the Instruction's execution status.
   *
   * @note Provided for thread safety.
   */
  void SetStatus(ExecutionStatus status);

  /**
   * @brief Private method that is always called before delegating further execution to the
   * private virutal Instruction::ExecuteSingleImple(UserInterface * ui, Workspace * ws).
   *
   * @details If the instruction was not yet executed it calls Instruction::InitHook(),
   * changes the status to ExecutionStatus::NOT_FINISHED and informs the user interface of a
   * status change.
   */
  void Preamble(UserInterface* ui);

  /**
   * @brief Private method that is always called the delegated execution to the
   * private virutal Instruction::ExecuteSingleImple(UserInterface * ui, Workspace * ws).
   *
   * @details This method first checks if the instruction's status was changed as a consequence
   * of the call to Instruction::ExecuteSingleImple(UserInterface * ui, Workspace * ws). If so,
   * it informs the user interface of this status change.
   */
  void Postamble(UserInterface* ui);

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
   * @details This hook is meant to establish a correct starting state (e.g. reset internal
   * counters) and will be called every time the instruction is executed afresh, i.e. restarted
   * from scratch. See Instruction::SetupImpl(const Procedure & proc) for establishing correct
   * configuration state of the instruction.
   * @note Default implementation is empty.
   */
  virtual void InitHook();

  /**
   * @brief Private implementation of the execute action.
   *
   * @details Pure virtual: this method contains the action(s) to be taken during execution.
   */
  virtual ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) = 0;

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
  virtual void ResetHook();

  /**
   * @brief Private hook that is called after variable initialisation.
   *
   * @return true on succes.
   * @details Mainly used to propagate attributes to nested instructions.
   * Default implementation returns true.
   */
  virtual bool PostInitialiseVariables(const AttributeMap& source);

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
  virtual std::vector<const Instruction *> ChildInstructionsImpl() const;

  /**
   * @brief Inserts child into the given index.
   *
   * @return true on success
   * @details Index must be in the range [0, current_children_count] inclusively.
   */
  virtual bool InsertInstructionImpl(Instruction* child, int index);

  /**
   * @brief Removes child with the given index and returns it to the user.
   *
   * @return child on success, nullptr otherwise
   * @details Index must be in the range [0, current_children_count-1], ownership
   * is given to the user.
   */
  virtual Instruction* TakeInstructionImpl(int index);
};

/**
 * @brief Construct a string prolog for throwing exceptions related to Instruction::Setup
 *
 * @param instruction Instruction to use.
 *
 * @return String containing the prolog to be used for the exception message.
 */
std::string InstructionSetupExceptionProlog(const Instruction& instruction);

/**
 * @brief Check if the instruction has an attribute with the given name and throw if not.
 *
 * @param instruction Instruction to check.
 * @param attr_name Mandatory attribute name.
 *
 * @throw InstructionSetupException when the instruction does not contain the attribute.
 */
void CheckMandatoryAttribute(const Instruction& instruction, const std::string& attr_name);

/**
 * @brief Check if the instruction has a non-empty attribute with the given name and throw if not.
 *
 * @param instruction Instruction to check.
 * @param attr_name Mandatory attribute name.
 *
 * @throw InstructionSetupException when the instruction does not contain the attribute or it is
 * empty.
 */
void CheckMandatoryNonEmptyAttribute(const Instruction& instruction, const std::string& attr_name);

/**
 * @brief Try to parse attribute to int and throw otherwise.
 *
 * @param instruction Instruction to check.
 * @param attr_name Attribute name.
 *
 * @return Parsed integer if successful.
 * @throw InstructionSetupException when the attribute could not be parsed into an int.
 */
int InstructionAttributeToInt(const Instruction& instruction, const std::string& attr_name);

/**
 * @brief Try to parse attribute to double and throw otherwise.
 *
 * @param instruction Instruction to check.
 * @param attr_name Attribute name.
 *
 * @return Parsed double if successful.
 * @throw InstructionSetupException when the attribute could not be parsed into an double.
 */
double InstructionAttributeToDouble(const Instruction& instruction, const std::string& attr_name);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_INSTRUCTION_H_
