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
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_BREAKPOINT_H_
#define SUP_SEQUENCER_BREAKPOINT_H_

namespace sup
{
namespace sequencer
{
class Instruction;
/**
 * @brief Class that manages a single breakpoint.
 */
class Breakpoint
{
public:
  enum Status {
    kSet = 0,   // stop execution when breakpoint is hit
    kReleased,  // continue execution and change status to kSet
  };

  Breakpoint(const Instruction* instruction);
  ~Breakpoint();

  const Instruction* GetInstruction() const;
  Status GetStatus() const;
  void SetStatus(const Status& status);

private:
  const Instruction* m_instruction;
  Status m_status;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_BREAKPOINT_H_
