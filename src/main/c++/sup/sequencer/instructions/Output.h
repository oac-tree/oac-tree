/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Gennady Pospelov (IO)
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
 * @file Output.h
 * @brief Header file for Output class.
 * @date 17/03/2021
 * @author Gennady Pospelov (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the Output class.
 */

#ifndef _SEQ_Output_h_
#define _SEQ_Output_h_

// Global header files

// Local header files

#include <sup/sequencer/instruction.h>

// Constants

// Type definition

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction node that put a workspace value into a user interface.
 */

class Output : public Instruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Execution returns SUCCESS if workspace value was successfully
   * put to the user interface.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

protected:
public:
  /**
   * @brief Constructor.
   */
  Output();

  /**
   * @brief Destructor.
   */
  ~Output() override;

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

#endif  // _SEQ_Input_h_
