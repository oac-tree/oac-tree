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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_BREAKPOINT_MANAGER_H_
#define SUP_SEQUENCER_BREAKPOINT_MANAGER_H_

#include <list>
#include <set>
#include <vector>

namespace sup
{
namespace sequencer
{
class Breakpoint;
class Instruction;
class Procedure;
/**
 * @brief Class that manages a set of breakpoints.
 */
class BreakpointManager
{
public:
  BreakpointManager();
  explicit BreakpointManager(const Procedure& proc);
  ~BreakpointManager();

  void SetBreakpoint(const Instruction* instruction);

  void RemoveBreakpoint(const Instruction* instruction);

  std::vector<const Instruction*> HandleBreakpoints(
    const std::vector<const Instruction*>& next_instructions);

  void ResetBreakpoints();

  std::list<Breakpoint> GetBreakpoints() const;

private:
  bool IsKnownInstruction(const Instruction* instruction) const;
  const std::set<const Instruction*> m_instruction_list;
  std::list<Breakpoint> m_breakpoints;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_BREAKPOINT_MANAGER_H_
