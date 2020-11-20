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
 * @file Workspace.h
 * @brief Header file for Workspace class.
 * @date dd/MM/YYYY
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the _____ class.
 */

#ifndef _SEQ_Workspace_h_
#define _SEQ_Workspace_h_

// Global header files

#include <map>
#include <vector>

// Local header files

#include "Variable.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Container class for managing variables.
 */

class Workspace
{
  private:
    std::map<std::string, Variable> _var_map;

  protected:

  public:
    /**
     * @brief Constructor.
     */
    Workspace();

    /**
     * @brief Destructor.
     */
    ~Workspace();

    /**
     * @brief Add variable.
     *
     * @param var Variable to add.
     */
    bool AddVariable(Variable var);

    /**
     * @brief List all variable names.
     */
    std::vector<std::string> VariableNames() const;

    /**
     * @brief Get variable value.
     *
     * @param name Variable name.
     */
    int GetVariableValue(std::string name);

    /**
     * @brief Set variable value
     *
     * @param name Variable name.
     * @param name Variable value.
     */
    bool SetVariableValue(std::string name, int value);
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

#endif // _SEQ_Workspace_h_
