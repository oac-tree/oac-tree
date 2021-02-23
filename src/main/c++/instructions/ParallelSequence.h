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
 * @file ParallelSequence.h
 * @brief Header file for ParallelSequence class.
 * @date 28/01/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2021 ITER Organization
 * @details This header file contains the definition of the ParallelSequence class.
 */

#ifndef _SEQ_ParallelSequence_h_
#define _SEQ_ParallelSequence_h_

// Global header files

// Local header files

#include "AsyncWrapper.h"
#include "CompoundInstruction.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Compound instruction representing a parallel sequence.
 *
 * @note ParallelSequence runs its child instructions concurrently and
 * returns SUCCESS when enough child instructions finish successfully (greater
 * than or equal to the success threshold). It fails when the failure threshold
 * is reached or the success threshold can no longer be reached. It returns
 * RUNNING otherwise.
 */
class ParallelSequence : public CompoundInstruction
{
  private:
    void InitHook() override;

    ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws) override;

    ExecutionStatus CalculateCompoundStatus() const;

    bool InitWrappers();

    std::vector<AsyncWrapper> _wrappers;

    int _success_th, _failure_th;

  public:
    /**
     * @brief Constructor.
     */
    ParallelSequence();

    /**
     * @brief Destructor.
     */
    ~ParallelSequence() override;

    bool Setup(Workspace * ws) override;

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

#endif // _SEQ_ParallelSequence_h_
