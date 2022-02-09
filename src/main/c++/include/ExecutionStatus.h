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
 * Copyright (c) : 2010-2022 ITER Organization,
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
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the ExecutionStatus class.
 */

#ifndef _SEQ_ExecutionStatus_h_
#define _SEQ_ExecutionStatus_h_

#include <string>

namespace sup
{
namespace sequencer
{
/**
 * @brief Enumeration for possible execution statuses.
 */
enum class ExecutionStatus
{
  NOT_STARTED,   // Not started yet.
  NOT_FINISHED,  // Started and waiting for next tick.
  RUNNING,       // Started and (child) running asynchronously.
  SUCCESS,       // Finished successfully.
  FAILURE        // Finished unsuccessfully.
};

/**
 * @brief Return string representation of ExecutionStatus enumerator.
 */
std::string StatusToString(const ExecutionStatus& status);

/**
 * @brief Check if the given status indicates the instruction was finished.
 * @return true if status is SUCCESS or FAILURE.
 */
bool IsFinishedStatus(const ExecutionStatus& status);

/**
 * @brief Check if the given status indicates further execution is required.
 * @return true if status is not SUCCESS nor FAILURE.
 */
bool NeedsExecute(const ExecutionStatus& status);

}  // namespace sequencer

}  // namespace sup

#endif  // _SEQ_ExecutionStatus_h_
