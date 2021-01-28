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
 * @file SuccessNode.h
 * @brief Header file for SuccessNode class.
 * @date 23/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the SuccessNode class.
 */

#ifndef _SEQ_SuccessNode_h_
#define _SEQ_SuccessNode_h_

// Global header files

// Local header files

#include "Instruction.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Instruction node that always returns SUCCESS immediately.
 */

class SuccessNode : public Instruction
{
  private:
    /**
     * @brief Timeout (in seconds)
     */
    double _timeout;

    /**
     * @brief Initialize optional timeout
     */
    virtual void InitHook();

    /**
     * @brief Execution always returns SUCCESS after an optional timeout.
     *
     * @return SUCCESS status.
     */
    ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws) override;

  protected:

  public:
    /**
     * @brief Constructor.
     */
    SuccessNode();

    /**
     * @brief Destructor.
     */
    ~SuccessNode() override;

    static const std::string Type;
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

#endif // _SEQ_SuccessNode_h_
