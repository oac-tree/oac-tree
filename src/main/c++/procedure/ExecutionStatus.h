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
 * @file ExecutionStatus.h
 * @brief Header file for ExecutionStatus class.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the ExecutionStatus class.
 */

#ifndef _SEQ_ExecutionStatus_h_
#define _SEQ_ExecutionStatus_h_

// Global header files

#include <string>

// Local header files

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Enumeration for possible execution statuses.
 */
enum class ExecutionStatus {
    UNDEFINED,
    STARTED,
    RUNNING,
    SUCCESS,
    FAILURE
};

// Global variables

// Function declarations

std::string StatusToString(const ExecutionStatus & status);

// Function definitions

} // namespace sequencer

} // namespace sup

extern "C" {
#endif // __cplusplus

// C API function declarations

#ifdef __cplusplus
} // extern C
#endif // __cplusplus

#endif // _SEQ_ExecutionStatus_h_
