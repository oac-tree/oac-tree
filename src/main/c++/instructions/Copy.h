/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project       : SUP - Sequencer
*
* Description   : Sequencer for operational procedures
*
* Author        : Bertrand Bauvir (IO)
*
* Copyright (c) : 2010-2021 ITER Organization,
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
 * @file Copy.h
 * @brief Header file for Copy class.
 * @date 11/03/2021
 * @author Bertrand Bauvir (IO)
 * @copyright 2010-2021 ITER Organization
 * @details This header file contains the definition of the Copy class.
 */

#ifndef _SEQ_Copy_h_
#define _SEQ_Copy_h_

// Global header files

// Local header files

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Instruction copying named variable from/to workspace
 */
class Copy : public Instruction
{
  private:
    /**
     * @brief See sup::sequencer::Instruction.
     *
     * @details Copy variables identified with 'input' and 'output' attributes.
     */
    ExecutionStatus ExecuteSingleImpl (UserInterface * ui, Workspace * ws) override;

  protected:

  public:
    /**
     * @brief Constructor.
     */
    Copy ();

    /**
     * @brief Destructor.
     */
    ~Copy () override;

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

#endif // _SEQ_Copy_h_
