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
 * @file AsyncWrapper.h
 * @brief Header file for AsyncWrapper class.
 * @date 27/01/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the AsyncWrapper class.
 */

#ifndef _SEQ_AsyncWrapper_h_
#define _SEQ_AsyncWrapper_h_

// Global header files

#include <future>
#include <memory>

// Local header files

#include "Instruction.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
/**
 * @brief Wrapper to execute an Instruction asynchronously.
 */

class AsyncWrapper
{
private:
protected:
  /**
   * @brief Wrapped instruction (not owned!)
   */
  Instruction* _instruction;

  /**
   * @brief Execution status of the wrapper
   */
  ExecutionStatus _status;

  /**
   * @brief Track the wrapped instruction
   */
  std::future<void> _child_result;

  /**
   * @brief Execution status of the wrapper
   */
  void LaunchChild(UserInterface* ui, Workspace* ws);

  /**
   * @brief Check if child is still running
   */
  bool ChildIsRunning() const;

public:
  /**
   * @brief Constructor.
   * @param instruction The wrapped Instruction.
   */
  AsyncWrapper(Instruction* instruction);

  /**
   * @brief Move constructor.
   *
   * @note The move constructor is needed for storage in STL containers.
   */
  AsyncWrapper(AsyncWrapper&& other);

  /**
   * @brief Destructor.
   */
  ~AsyncWrapper();

  /**
   * @brief Execution method.
   *
   * @details Launches the ExecuteSingle method on the wrapped instruction
   * asynchronously if it is not already running.
   */
  void Tick(UserInterface* ui, Workspace* ws);

  /**
   * @brief Get execution status
   */
  ExecutionStatus GetStatus() const;
};

// Global variables

// Function declarations

// Function definitions

}  // namespace sequencer

}  // namespace sup

extern "C"
{
#endif  // __cplusplus

  // C API function declarations

#ifdef __cplusplus
}  // extern C
#endif  // __cplusplus

#endif  // _SEQ_AsyncWrapper_h_
