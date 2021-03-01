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

// Local header files

#include "AttributeMap.h"
#include "ExecutionStatus.h"

// Constants

#ifdef __cplusplus

namespace sup {

namespace sequencer {

// Forward declarations

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

  protected:

  public:
    /**
     * @brief Constructor
     * @param type The type of instruction.
     */
    Instruction(const std::string & type);

    /**
     * @brief Virtual destructor
     */
    virtual ~Instruction();

    /**
     * @brief Get instruction type
     * @return instruction type
     */
    std::string GetType() const;

    /**
     * @brief Get instruction name
     * @return instruction name
     */
    std::string GetName() const;

    /**
     * @brief Set instruction name
     * @param name Name to set
     * @return void
     */
    void SetName(const std::string & name);

    /**
     * @brief Setup method
     * @details
     * @param
     * @return
     */
    virtual bool Setup(Workspace * ws);

    /**
     * @brief Execution method
     * @details
     * @param
     * @return
     */
    void ExecuteSingle(UserInterface * ui, Workspace * ws);

    /**
     * @brief Get execution status
     */
    ExecutionStatus GetStatus() const;

    /**
     * @brief Reset execution status
     */
    void ResetStatus();

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
     * @brief Set attribute with given name and value.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return true when successful.
     */
    bool AddAttribute(const std::string & name, const std::string & value);
};

// Global variables

// Function declarations

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
