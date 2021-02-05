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
 * @file Runner.h
 * @brief Header file for Runner class.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the Runner class.
 */

#ifndef _SEQ_Runner_h_
#define _SEQ_Runner_h_

// Global header files

// Local header files

// Constants

#ifdef __cplusplus

namespace sup {

namespace sequencer {

// Forward declarations

class Procedure;
class UserInterface;

// Type definition

/**
 * @brief Class providing .
 */
class Runner
{
  private:
    Procedure * _proc;

    UserInterface * _ui;

  protected:

  public:
    /**
     * @brief Constructor.
     * @param ui User interface object.
     */
    Runner(UserInterface * ui);

    /**
     * @brief Destructor.
     */
    ~Runner();

    /**
     * @brief Set the procedure..
     * @param procedure Pointer to procedure.
     */
    void SetProcedure(Procedure * procedure);

    /**
     * @brief Execute the procedure
     */
    void ExecuteProcedure();

    /**
     * @brief Execute single procedure instruction.
     */
    void ExecuteSingle();

    /**
     * @brief Query if procedure has finished.
     *
     * @return True if procedure has finished.
     */
    bool IsFinished() const;

    /**
     * @brief Query if procedure is running in seperate threads.
     *
     * @return True if procedure has seperate threads executing.
     */
    bool IsRunning() const;
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

#endif // _SEQ_Runner_h_
