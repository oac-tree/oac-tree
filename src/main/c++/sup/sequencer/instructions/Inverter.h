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
 * @file Inverter.h
 * @brief Header file for Inverter class.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the Inverter class.
 */

#ifndef _SEQ_Inverter_h_
#define _SEQ_Inverter_h_

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
 * @brief Decorator that inverts the execution status of its child.
 */
class Inverter : public DecoratorInstruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Inverts the execution status of its child instruction, interchanging
   * SUCCESS and FAILURE.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

  /**
   * @brief Calculate this instruction's status from the status of its child instruction.
   */
  ExecutionStatus CalculateStatus() const;

protected:
public:
  /**
   * @brief Constructor.
   */
  Inverter();

  /**
   * @brief Destructor.
   */
  ~Inverter() override;

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

#endif  // _SEQ_Inverter_h_
