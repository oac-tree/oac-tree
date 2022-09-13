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
 * @file Input.h
 * @brief Header file for Input class.
 * @date 17/03/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the Input class.
 */

#ifndef _SEQ_Input_h_
#define _SEQ_Input_h_

// Global header files

// Local header files

#include <sup/sequencer/Instruction.h>

// Constants

// Type definition

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction node that writes a user defined value into a workspace
 * variable.
 */

class Input : public Instruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Execution returns SUCCESS if user defined value was successfully
   * written to the workspace variable.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

protected:
public:
  /**
   * @brief Constructor.
   */
  Input();

  /**
   * @brief Destructor.
   */
  ~Input() override;

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
