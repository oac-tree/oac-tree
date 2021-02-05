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
 * @file UserInterface.h
 * @brief Header file for UserInterface class.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the UserInterface class.
 */

#ifndef _SEQ_UserInterface_h_
#define _SEQ_UserInterface_h_

// Global header files

// Local header files

// Constants

#ifdef __cplusplus

namespace sup {

namespace sequencer {

// Forward declarations

class Instruction;

// Type definition

/**
 * @brief Interface representing user interactions.
 */
class UserInterface
{
  private:

  protected:

  public:
    /**
     * @brief Constructor.
     */
    UserInterface() = default;

    /**
     * @brief Destructor.
     */
    virtual ~UserInterface() = default;

    /**
     * @brief Method called when instruction's execution status changes.
     *
     * @param instruction Instruction that has new execution status.
     */
    virtual void UpdateInstructionStatus(const Instruction * instruction) = 0;

    /**
     * @brief Method called before stepping into an instruction.
     */
    virtual void StartSingleStep();

    /**
     * @brief Method called after executing an instruction.
     */
    virtual void EndSingleStep();
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

#endif // _SEQ_UserInterface_h_
