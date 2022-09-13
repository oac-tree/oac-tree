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
 * @file Condition.h
 * @brief Header file for Condition class.
 * @date 15/01/2021
 * @author Giuseppe Ferro (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the Condition class.
 */

#ifndef _SEQ_Condition_h_
#define _SEQ_Condition_h_

// Global header files

// Local header files

#include <sup/sequencer/instruction.h>

// Constants

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
// Forward declarations

// Type definition

/**
 * @brief Checks a boolean workspace variable.
 */
class Condition : public Instruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Returns SUCCESS if the variable is true and FAILURE otherwise. If the variable
   * is not boolean, it will check if the value is different than 0
   */
  virtual ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws);

public:
  /**
   * @brief Constructor.
   */
  Condition();

  /**
   * @brief Destructor.
   */
  virtual ~Condition();

  /**
   * @brief The instruction's typename.
   */
  static const std::string Type;
};

}  // namespace sequencer

}  // namespace sup

extern "C"
{
#endif  // __cplusplus

  // C API function declarations

#ifdef __cplusplus
}  // extern C
#endif  // __cplusplus

#endif  // _SEQ_Condition_h_
