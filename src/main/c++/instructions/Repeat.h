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
 * @file Repeat.h
 * @brief Header file for Repeat instruction class.
 * @date 16/12/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the Repeat class.
 */

#ifndef _SEQ_Repeat_h_
#define _SEQ_Repeat_h_

// Global header files

// Local header files

#include "DecoratorInstruction.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction decorator that repeats its child a fixed number of times while successful.
 */

class Repeat : public DecoratorInstruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   */
  void InitHook() override;

  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Repeatedly execute the child instruction, until either: the child fails (FAILURE)
   * or maximum number of repetitions is reached (SUCCESS).
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

  /**
   * @brief See sup::sequencer::Instruction.
   */
  bool SetupImpl(const Procedure& proc) override;

  /**
   * @brief Calculate this instruction's status from the status of its child instruction
   * and the number of successful executions of the child instruction.
   */
  ExecutionStatus CalculateStatus() const;

  int _max_count, _count;

  bool _init_ok;

protected:
public:
  /**
   * @brief Constructor.
   */
  Repeat();

  /**
   * @brief Destructor.
   */
  ~Repeat() override;

  /**
   * @brief The instruction's typename.
   */
  static const std::string Type;
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

#endif  // _SEQ_Repeat_h_
