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

#ifndef SUP_SEQUENCER_EQUALS_H_
#define SUP_SEQUENCER_EQUALS_H_

#include <sup/sequencer/instruction.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction checking the equality of two variables
 */
class Equals : public Instruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Check equality of variables identified with 'lhs' and 'rhs' attributes.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

protected:
public:
  /**
   * @brief Constructor.
   */
  Equals();

  /**
   * @brief Destructor.
   */
  ~Equals() override;

  /**
   * @brief The instruction's typename.
   */
  static const std::string Type;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_EQUALS_H_
