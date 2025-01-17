/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Ricardo Torres (EXT)
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

#ifndef SUP_OAC_TREE_WAIT_FOR_VARIABLES_H_
#define SUP_OAC_TREE_WAIT_FOR_VARIABLES_H_

#include <sup/oac-tree/instruction.h>

#include <sup/dto/basic_scalar_types.h>

#include <condition_variable>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction node that waits 'timeout' seconds for all variables of a certain type to be
 * available, i.e. as defined by Variable::IsAvailable().
 *
 * @details The 'timeout' attribute is mandatory.
 */
class WaitForVariables : public Instruction
{
public:
  WaitForVariables();

  ~WaitForVariables() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  /**
   * @brief Register variable callbacks for all variables concerned.
   *
   * @param ws Workspace to use.
   * @param cv Condition variable that will be notified.
   * @param listener Listener pointer to use.
   * @param var_names List of variable names to be used for callbacks.
   */
  void RegisterCallbacks(Workspace& ws, std::condition_variable& cv,
                         void* listener, const std::vector<std::string>& var_names) const;

  /**
   * @brief Return subset of variable names that are not available.
   *
   * @param ws Workspace to use.
   * @param var_names Variable names to check for availability.
   * @return List of variable names that are not available.
   */
  std::vector<std::string> UnavailableVars(Workspace& ws,
                                           const std::vector<std::string>& var_names) const;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_WAIT_FOR_VARIABLES_H_
