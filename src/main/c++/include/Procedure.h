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
 * @file Procedure.h
 * @brief Header file for Procedure class.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the Procedure class.
 */

#ifndef _SEQ_Procedure_h_
#define _SEQ_Procedure_h_

// Global header files

#include <memory>
#include <vector>

#include <common/AnyValue.h>

// Local header files

#include "ExecutionStatus.h"

// Constants

#ifdef __cplusplus

namespace sup {

namespace sequencer {

// Forward declarations

class AttributeMap;
class Instruction;
class UserInterface;
class Variable;
class Workspace;

// Type definition

/**
 * @brief Procedure contains a tree of instructions
 *
 * @detail A Procedure object contains a full procedure
 * and a workspace
 */
class Procedure
{
  private:
    std::unique_ptr<Instruction> _root;
    std::unique_ptr<Workspace> _workspace;

    std::unique_ptr<AttributeMap> _attributes;

  protected:

  public:
    /**
     * @brief Constructor
     */
    Procedure();

    /**
     * @brief Destructor
     */
    ~Procedure();

    /**
     * @brief Add variable.
     *
     * @param name Variable name.
     * @param var Variable to add.
     */
    bool AddVariable(std::string name, Variable * var);

    /**
     * @brief List all variable names
     */
    std::vector<std::string> VariableNames() const;

    /**
     * @brief Get variable value.
     */
    bool GetVariableValue(std::string name, ::ccs::types::AnyValue& value);

    /**
     * @brief Set the root instruction.
     *
     * @param instruction Root instruction to set.
     */
    bool SetRootInstruction(Instruction * instruction);

    /**
     * @brief Push Instruction at top level.
     */
    bool PushInstruction(Instruction * instruction);

    /**
     * @brief Execute single step of procedure
     */
    void ExecuteSingle(UserInterface * ui);

    /**
     * @brief Retrieve status of root sequence
     */
    ExecutionStatus GetStatus() const;

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

// Function definitions

} // namespace sequencer

} // namespace sup

extern "C" {
#endif // __cplusplus

// C API function declarations

#ifdef __cplusplus
} // extern C
#endif // __cplusplus

#endif // _SEQ_Procedure_h_
