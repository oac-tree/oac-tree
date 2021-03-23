/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project       : SUP - Sequencer
*
* Description   : Test helper routines
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
 * @file UnitTestHelper.h
 * @brief Header file for heper routines.
 * @date 17/03/21
 * @author Bertrand Bauvir (IO)
 * @copyright 2010-2021 ITER Organization
 * @details This header file contains the definition of misc. helper routines.
 */

#ifndef _SEQ_UnitTestHelper_h_
#define _SEQ_UnitTestHelper_h_

// Global header files

// Local header files

#include "ExecutionStatus.h"
#include "Procedure.h"
#include "UserInterface.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

// Global variables

// Function declarations

static inline bool TryAndExecute (std::unique_ptr<sup::sequencer::Procedure>& proc, sup::sequencer::UserInterface * const ui, const sup::sequencer::ExecutionStatus& expect = sup::sequencer::ExecutionStatus::SUCCESS);

// Function definitions

static inline bool TryAndExecute (std::unique_ptr<sup::sequencer::Procedure>& proc, sup::sequencer::UserInterface * const ui, const sup::sequencer::ExecutionStatus& expect)
{

  bool status = static_cast<bool>(proc);

  if (status)
    { // Setup procedure
      status = proc->Setup();
    }

  if (status)
    {
      sup::sequencer::ExecutionStatus exec = sup::sequencer::ExecutionStatus::FAILURE;

      do
        {
          (void)ccs::HelperTools::SleepFor(100000000ul); // Let system breathe
          proc->ExecuteSingle(ui);
          exec = proc->GetStatus();
        }
      while ((sup::sequencer::ExecutionStatus::SUCCESS != exec) &&
             (sup::sequencer::ExecutionStatus::FAILURE != exec));

      status = (expect == exec);

      proc->Reset(); // Wait for thread termination before calling destructor of UI
    }

  return status;

}

} // namespace sequencer

} // namespace sup

extern "C" {
#endif // __cplusplus

// C API function declarations

#ifdef __cplusplus
} // extern C
#endif // __cplusplus

#endif // _SEQ_UnitTestHelper_h_
