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
 * @file Instruction.h
 * @brief Header file for Instruction interface.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the Instruction interface.
 */

#ifndef _SEQ_Instruction_h_
#define _SEQ_Instruction_h_

// Global header files

#include <atomic>
#include <mutex>

// Local header files

#include "AttributeMap.h"
#include "ExecutionStatus.h"

// Constants

#ifdef __cplusplus

namespace sup {

namespace sequencer {

// Forward declarations

class Procedure;
class UserInterface;
class Workspace;

// Type definition

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
     * @brief Private method that is always called before delegating further execution to the
     * private virutal Instruction::ExecuteSingleImple(UserInterface * ui, Workspace * ws).
     *
     * @details This method first calls the virtual private Instruction::PreExecuteHook(UserInterface * ui).
     * If the instruction was not yet executed (or reset through a call to Instruction::Reset()),
     * it clears the halt requested atomic, calls Instruction::InitHook(), changes the status to
     * ExecutionStatus::NOT_FINISHED and finally informs the user interface of a status change.
     */
    void Preamble(UserInterface * ui);

    /**
     * @brief Private hook that is always called at the start of each execution request.
     *
     * @details See Instruction::Preamble(UserInterface * ui) for more details.
     * @note Default implementation is empty.
     */
    virtual void PreExecuteHook(UserInterface * ui);

    /**
     * @brief Private implementation of the execute action.
     *
     * @details Pure virtual: this method contains the action(s) to be taken during execution.
     */
    virtual ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws) = 0;

    /**
     * @brief Private hook that is always called at the end of each execution request.
     *
     * @details See Instruction::Postamble(UserInterface * ui) for more details.
     * @note Default implementation is empty.
     */
    virtual void PostExecuteHook(UserInterface * ui);

    /**
     * @brief Private method that is always called the delegated execution to the
     * private virutal Instruction::ExecuteSingleImple(UserInterface * ui, Workspace * ws).
     *
     * @details This method first checks if the instruction's status was changed as a consequence
     * of the call to Instruction::ExecuteSingleImple(UserInterface * ui, Workspace * ws). If so,
     * it informs the user interface of this status change. Afterwards, it calls the virtual
     * private Instruction::PostExecuteHook(UserInterface * ui).
     */
    void Postamble(UserInterface * ui);

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
     * @brief Hook called during Instruction::Halt().
     *
     * @details This hook is meant to propagate the call to Instruction::Halt() to
     * the instructions's child instructions.
     */
    virtual void HaltImpl();

    /**
     * @brief Private hook that is called after variable initialisation.
     *
     * @return true on succes.
     * @details Mainly used to propagate attributes to nested instructions.
     * Default implementation returns true.
     */
    virtual bool PostInitialiseVariables(const AttributeMap & source);

    /**
     * @brief Get list of child instructions implementation (const version).
     *
     * @return List of child instructions.
     */
    virtual std::vector<const Instruction *> ChildInstructionsImpl() const;

    /**
     * @brief Setup implementation.
     *
     * @param proc Procedure containing Workspace and instruction declarations.
     * @return true on successful instruction setup.
     * @details Successful completion of this instruction means that the instruction
     * was able to configure itself from its attributes and (possibly) data from the Procedure.
     * This method is called from the Instruction::Setup(const Procedure & proc) method.
     */
    virtual bool SetupImpl(const Procedure & proc);

  protected:
    /**
     * @brief Atomic flag that indicates if a request was made to halt the instruction.
     */
     std::atomic_bool _halt_requested;

  public:
    /**
     * @brief Constructor.
     * @param type The type of instruction.
     */
    Instruction(const std::string & type);

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
    void SetName(const std::string & name);

    /**
     * @brief Setup method.
     * @param proc Procedure containing Workspace and instruction declarations.
     * @return true on successful instruction setup.
     */
    bool Setup(const Procedure & proc);

    /**
     * @brief Execution method.
     * @param ui UserInterface to handle input/output.
     * @param ws Workspace that contains the variables.
     */
    void ExecuteSingle(UserInterface * ui, Workspace * ws);

    /**
     * @brief Halt execution.
     * @details Only meaningful when the instruction is running asynchronously.
     * This method only sets an atomic boolean member variable. It is up to implementations
     * of the Instruction to check this variable regularly to prevent long blocking.
     */
    void Halt();

    /**
     * @brief Get execution status.
     * @return Execution status.
     */
    ExecutionStatus GetStatus() const;

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
    bool HasAttribute(const std::string & name) const;

    /**
     * @brief Get attribute with given name.
     *
     * @param name Attribute name.
     * @return Attribute value.
     */
    std::string GetAttribute(const std::string & name) const;

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
    bool AddAttribute(const std::string & name, const std::string & value);

    /**
     * @brief Set all attributes from given map.
     *
     * @param attributes Map of attributes to set.
     * @return true when successful.
     */
    bool AddAttributes(const AttributeMap & attributes);

    /**
     * @brief Initialise variable attributes with values from attribute map.
     *
     * @param source Map containing variable name - value pairs.
     * @return true when all variable attributes were initialised.
     */
    bool InitialiseVariableAttributes(const AttributeMap & source);

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
};

// Global variables

// Function declarations

/**
 * @brief Check if status corresponds to a state where execution is meaningful.
 * @return true if execution is meaningful (not already finished).
 */
bool NeedsExecute(ExecutionStatus status);

// Function definitions

} // namespace sequencer

} // namespace sup

extern "C" {
#endif // __cplusplus

// C API function declarations

#ifdef __cplusplus
} // extern C
#endif // __cplusplus

#endif // _SEQ_Instruction_h_
