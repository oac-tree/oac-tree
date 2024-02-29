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
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_FALLBACK_H_
#define SUP_SEQUENCER_FALLBACK_H_

#include <sup/sequencer/compound_instruction.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Compound instruction representing a list of alternative actions.
 *
 * @note Fallback keeps on executing its children until one returns successfully.
 * The fallback sequence fails when all children fail.
 */
class Fallback : public CompoundInstruction
{
public:
  Fallback();

  ~Fallback() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  std::vector<const Instruction*> NextInstructionsImpl() const override;

  /**
   * @brief Calculate this instruction's status from the status of its child instructions.
   */
  ExecutionStatus CalculateCompoundStatus() const;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_FALLBACK_H_
