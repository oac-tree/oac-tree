/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_EXECUTION_STATUS_H_
#define SUP_OAC_TREE_EXECUTION_STATUS_H_

#include <string>

namespace sup
{
/**
 * @brief Main namespace for the SUP oac-tree
*/
namespace oac_tree
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

/**
 * @brief Check if the given status indicates the instruction is ready to be executed next.
 * @return true if status is NOT_STARTED or NOT_FINISHED.
 *
 * @details The different with NeedsExecute is that RUNNING is not considered to be ready for
 * execution, as it is not yet clear if the previous execution step finished already.
 */
bool ReadyForExecute(const ExecutionStatus& status);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_EXECUTION_STATUS_H_
