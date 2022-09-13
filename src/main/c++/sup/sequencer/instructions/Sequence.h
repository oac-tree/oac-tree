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
 * @file Sequence.h
 * @brief Header file for Sequence class.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the Sequence class.
 */

#ifndef _SEQ_Sequence_h_
#define _SEQ_Sequence_h_

// Global header files

// Local header files

#include "CompoundInstruction.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
/**
 * @brief Compound instruction representing a sequence.
 *
 * @note Sequence keeps on executing its children as long as they
 * return successfully. Sequence fails as soon as a child fails.
 */
class Sequence : public CompoundInstruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Execute the child instructions in order until one fails (FAILURE)
   * or all succeed (SUCCESS).
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

  /**
   * @brief Calculate this instruction's status from the status of its child instructions.
   */
  ExecutionStatus CalculateCompoundStatus() const;

public:
  /**
   * @brief Constructor.
   */
  Sequence();

  /**
   * @brief Destructor.
   */
  ~Sequence() override;

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

#endif  // _SEQ_Sequence_h_
