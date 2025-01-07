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

#ifndef SUP_SEQUENCER_BREAKPOINT_MANAGER_H_
#define SUP_SEQUENCER_BREAKPOINT_MANAGER_H_

#include <list>
#include <mutex>
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
  /**
   * @brief Default constructor creates a manager with no known instruction pointers.
   */
  BreakpointManager();

  /**
   * @brief Constructor that initializes its known instruction pointer list with those from the
   * given Procedure.
   *
   * @param proc Procedure whose instructions will be valid for breakpoints.
   */
  explicit BreakpointManager(const Procedure& proc);

  /**
   * @brief Destructor.
   */
  ~BreakpointManager();

  /**
   * @brief Set a Breakpoint at the given instruction.
   *
   * @param instruction Instruction for the breakpoint.
   * @return True when a new breakpoint was added, i.e. it wasn't already there.
   *
   * @throws InvalidOperationException when instruction is unknown.
   */
  bool SetBreakpoint(const Instruction* instruction);

  /**
   * @brief Remove a breakpoint at the given instruction.
   *
   * @param instruction Breakpoint instruction to remove.
   * @return True when an existing breakpoint was removed, false when there was no breakpoint
   * associated with the given instruction or the instruction was unknown.
   */
  bool RemoveBreakpoint(const Instruction* instruction);

  /**
   * @brief Check if there's a breakpoint set for any of the given instructions. If so, it releases
   * those breakpoints temporarily.
   *
   * @param next_instructions List of instructions to check with the set breakpoints.
   * @return List of instructions where breakpoints were found to be set.
   */
  std::vector<const Instruction*> HandleBreakpoints(
    const std::vector<const Instruction*>& next_instructions);

  /**
   * @brief Reset all released breakpoints.
   */
  void ResetBreakpoints();

  /**
   * @brief Get a list of managed breakpoints.
   *
   * @return List of breakpoints.
   */
  std::list<Breakpoint> GetBreakpoints() const;

private:
  /**
   * @brief Check if the given instruction is known, i.e. it is a valid instruction for a
   * breakpoint.
   *
   * @param instruction Instruction pointer to check.
   * @return true when the instruction is known, false otherwise.
   */
  bool IsKnownInstruction(const Instruction* instruction) const;

  const std::set<const Instruction*> m_instruction_list;
  std::list<Breakpoint> m_breakpoints;
  mutable std::mutex m_mtx;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_BREAKPOINT_MANAGER_H_
