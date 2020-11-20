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
 * @file
 * @brief Header file for .
 * @date dd/MM/YYYY
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the _____ class.
 */

#ifndef _SEQ_ProcedureData_h_
#define _SEQ_ProcedureData_h_

// Global header files

#include <memory>

// Local header files

#include "InstructionData.h"
#include "WorkspaceData.h"
#include "procedure/Procedure.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Class providing .
 */

class ProcedureData
{
  private:
    std::unique_ptr<InstructionData> _root;
    std::unique_ptr<WorkspaceData> _workspace;

  protected:

  public:
    /**
     * @brief Constructor.
     *
     * @param root Root instruction data.
     * @param ws_data Workspace data.
     */
  ProcedureData(InstructionData * root, WorkspaceData * ws_data);

    /**
     * @brief Destructor.
     */
  ~ProcedureData() = default;

    /**
     * @brief Create a procedure.
     *
     * @return Unique pointer to procedure.
     */
  std::unique_ptr<Procedure> CreateProcedure() const;
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

#endif // _SEQ_ProcedureData_h_
