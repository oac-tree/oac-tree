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

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <common/AnyValue.h>

// Local header files

#include "AttributeMap.h"
#include "ExecutionStatus.h"

// Constants

#ifdef __cplusplus

namespace sup {

namespace sequencer {

// Forward declarations

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
    std::vector<std::unique_ptr<Instruction>> _instructions;
    std::unique_ptr<Workspace> _workspace;

    AttributeMap _attributes;

    // Directory of current procedure (if loaded from file).
    std::string _current_directory;

    // Cache for other procedures loaded from files and to be used by include nodes.
    mutable std::map<std::string, std::unique_ptr<Procedure>> _procedure_cache;

    /**
     * @brief Load a procedure from file or cache.
     *
     * @param filename Filename of the procedure file.
     * @return Pointer to the procedure or nullptr in case of failure to load the procedure.
     *
     * @details This method returns a pointer to a const Procedure. The Procedure itself
     * is owned by the cache.
     */
    const Procedure * LoadProcedure(const std::string & filename) const;

    /**
     * @brief Clear the cached procedures.
     *
     * @details This method needs to be called anytime there is a possibility of changes
     * to the loaded files on disk.
     */
    void ClearProcedureCache() const;

  protected:

  public:
    /**
     * @brief Constructor.
     */
    Procedure();

    /**
     * @brief Destructor.
     */
    ~Procedure();

    /**
     * @brief Get root instruction.
     *
     * @return Root instruction.
     */
    Instruction * RootInstrunction();

    /**
     * @brief Set directory of current procedure (if loaded from file).
     *
     * @param directory Directory of current procedure.
     * @details This directory is used for external includes with relative pathnames.
     */
    void SetCurrentDirectory(const std::string & directory);

    /**
     * @brief Get directory of current procedure (if loaded from file).
     *
     * @return Directory of current procedure.
     * @details This directory is used for external includes with relative pathnames.
     */
    std::string GetCurrentDirectory() const;

    /**
     * @brief Get root instruction (const version).
     *
     * @return Root instruction.
     */
    const Instruction * RootInstrunction() const;

    /**
     * @brief Get top-level instructions (from other procedure file if requested).
     *
     * @param filename Optional filename for external loading of instructions.
     * @return List of top-level instructions.
     * @details If the filename argument is not empty, this method will first look into the
     * procedure cache to see if this file was already loaded. If not, it will load it into the
     * cache and then return its top-level instructions.
     */
    std::vector<const Instruction *> GetInstructions(const std::string & filename = {}) const;

    /**
     * @brief Push Instruction at top level.
     */
    bool PushInstruction(Instruction * instruction);

    /**
     * @brief Add variable.
     *
     * @param name Variable name.
     * @param var Variable to add.
     * @return true on successful addition.
     */
    bool AddVariable(std::string name, Variable * var);

    /**
     * @brief List all variable names.
     *
     * @return Variable name list.
     */
    std::vector<std::string> VariableNames() const;

    /**
     * @brief Get variable value.
     *
     * @return true on successful retrieval.
     */
    bool GetVariableValue(std::string name, ::ccs::types::AnyValue & value);

    /**
     * @brief Setup the procedure.
     *
     * @return true on successful setup.
     */
    bool Setup();

    /**
     * @brief Execute single step of procedure
     *
     * @param ui UserInterface to use for instruction input/output.
     */
    void ExecuteSingle(UserInterface * ui);

    /**
     * @brief Retrieve status of root sequence.
     *
     * @return Current execution status of the root instruction.
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

    /**
     * @brief Add all attributes from a given map.
     *
     * @param attributes Attribute map.
     * @return true when successful.
     */
    bool AddAttributes(const AttributeMap & attributes);

    /**
     * @brief Name of attribute that defines the timeout between ticks.
     */
    static const std::string TICK_TIMEOUT_ATTRIBUTE_NAME;
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
