/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
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

#ifndef SUP_OAC_TREE_JOB_COMMANDS_H_
#define SUP_OAC_TREE_JOB_COMMANDS_H_

namespace sup
{
namespace oac_tree
{
enum class JobCommand
{
  kStart = 0,
  kStep,
  kPause,
  kReset,
  kHalt,
  kTerminate
};

bool operator<=(JobCommand left, JobCommand right);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_JOB_COMMANDS_H_
