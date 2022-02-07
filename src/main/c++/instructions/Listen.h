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
 * @file Listen.h
 * @brief Header file for Listen class.
 * @date 07/02/2022
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the Listen class.
 */

#ifndef _SEQ_Listen_h_
#define _SEQ_Listen_h_

#include "DecoratorInstruction.h"

namespace sup
{
namespace sequencer
{
/**
 * @brief Decorator that executes its child instruction each time specific variables are updated.
 */
class Listen : public DecoratorInstruction
{
public:
  /**
   * @brief Constructor.
   */
  Listen();

  /**
   * @brief Destructor.
   */
  ~Listen() override;

  /**
   * @brief The instruction's typename.
   */
  static const std::string Type;

private:
  bool force_success;

  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Repeatedly execute the child instruction when specific variables are updated.
   * When forceSuccess is not set, the instruction only ends on the child's FAILURE and returns
   * FAILURE accordingly.
   * The instruction also immediately returns SUCCESS in the absence of a child instruction.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Initializes the instruction's attributes.
   */
  bool SetupImpl(const Procedure& proc) override;

  std::vector<std::string> VariableNames() const;
};

}  // namespace sequencer

}  // namespace sup

#endif  // _SEQ_Listen_h_
