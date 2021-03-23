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
 * @details A Procedure object contains a full instruction tree and a workspace
 * @note The client of the Procedure, i.e. the object responsible for its creation and
 * destruction, needs to ensure that the Procedure is correctly set up before executing it (by
 * calling the Setup method on it). Likewise, the client needs to call Reset on the Procedure
 * before destroying the UserInterface class. Reset will block until all threads have terminated.
 */
class Procedure
{
  private:
    std::vector<std::unique_ptr<Instruction>> _instructions;
    std::unique_ptr<Workspace> _workspace;

    AttributeMap _attributes;

    /**
     * @brief Name of file from which this procedure was loaded (if loaded form file).
     *
     * @note This filename may include the relative or absolute paths.
     */
    std::string _filename;

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
     * @brief Set the filename of this procedure (if loaded from file).
     *
     * @param filename Filename of this procedure.
     * @details This filename is used for external includes with relative pathnames.
     */
    void SetFilename(const std::string & filename);

    /**
     * @brief Get the filename of this procedure (if loaded from file).
     *
     * @return Filename of this procedure.
     * @details This filename is used for external includes with relative pathnames.
     */
    std::string GetFilename() const;

    /**
     * @brief Get root instruction.
     *
     * @return Root instruction.
     */
    Instruction * RootInstrunction();

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
     *
     * @note Procedure takes ownership of the instruction and will take care of its timely
     * destruction. This implies that the client code should NEVER push the same
     * instruction more than once.
     */
    bool PushInstruction(Instruction * instruction);

    /**
     * @brief Add variable.
     *
     * @param name Variable name.
     * @param var Variable to add.
     * @return true on successful addition.
     * @note Procedure, or rather its underlying Workspace, takes ownership of the
     * variable and will take care of its timely destruction.
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
    bool GetVariableValue(std::string name, ::ccs::types::AnyValue & value) const;

    /**
     * @brief Setup the procedure.
     *
     * @return true on successful setup.
     */
    bool Setup();

    /**
     * @brief Execute single step of procedure.
     *
     * @param ui UserInterface to use for instruction input/output.
     */
    void ExecuteSingle(UserInterface * ui);

    /**
     * @brief Reset procedure.
     *
     * @details This method request a halt and blocks until all asynchronous instructions are finished.
     */
    void Reset();

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
