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
 * @file Wait.h
 * @brief Header file for Wait class.
 * @date 23/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the Wait class.
 */

#ifndef _SEQ_Wait_h_
#define _SEQ_Wait_h_

// Global header files

// Local header files

#include "Instruction.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Instruction node that returns SUCCESS after a given timeout.
 */

class Wait : public Instruction
{
  private:
    /**
     * @brief See sup::sequencer::Instruction.
     *
     * @details Execution always returns SUCCESS after an optional timeout.
     */
    ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws) override;

    /**
     * @brief Timeout (in seconds)
     */
    double _timeout;

  protected:
    /**
     * @brief See sup::sequencer::Instruction.
     */
    bool SetupImpl(const Procedure & proc) override;

  public:
    /**
     * @brief Constructor.
     */
    Wait();

    /**
     * @brief Destructor.
     */
    ~Wait() override;

    /**
     * @brief The instruction's typename.
     */
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

#endif // _SEQ_Wait_h_
