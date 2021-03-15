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
    std::string _type;

    ExecutionStatus _status;

    ExecutionStatus _status_before;

    AttributeMap _attributes;

    virtual void InitHook();

    void Preamble(UserInterface * ui);

    virtual void PreExecuteHook(UserInterface * ui);

    virtual ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws) = 0;

    virtual void PostExecuteHook(UserInterface * ui);

    void Postamble(UserInterface * ui);

    virtual void ResetHook();

    virtual void HaltImpl();

    /**
     * @brief Private hook that is called after variable initialisation.
     *
     * @return true on succes.
     *
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

  protected:
    /**
     * @brief Setup implementation.
     */
     std::atomic_bool _halt_requested;

    /**
     * @brief Setup implementation.
     *
     * @param proc Procedure containing Workspace and instruction declarations.
     * @return true on successful instruction setup.
     */
    virtual bool SetupImpl(const Procedure & proc);

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
     * @return instruction name.
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
     * @details Only meaningful for asynchronously running instructions.
     */
    void Halt();

    /**
     * @brief Get execution status.
     * @return Execution status.
     */
    ExecutionStatus GetStatus() const;

    /**
     * @brief Reset execution status
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
