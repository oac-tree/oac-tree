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
 * @file Fallback.h
 * @brief Header file for Fallback class.
 * @date 12/02/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2021 ITER Organization
 * @details This header file contains the definition of the Fallback class.
 */

#ifndef _SEQ_Fallback_h_
#define _SEQ_Fallback_h_

// Global header files

// Local header files

#include "CompoundInstruction.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Compound instruction representing a list of alternative actions.
 *
 * @note Fallback keeps on executing its children until one
 * returns successfully. Sequence fails when all children fail.
 */
class Fallback : public CompoundInstruction
{
  private:
    ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws) override;

    ExecutionStatus CalculateCompoundStatus() const;

  public:
    /**
     * @brief Constructor.
     */
    Fallback();

    /**
     * @brief Destructor.
     */
    ~Fallback() override;

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

#endif // _SEQ_Fallback_h_
