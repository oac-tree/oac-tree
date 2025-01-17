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

#ifndef SUP_OAC_TREE_JOB_INTERFACE_H_
#define SUP_OAC_TREE_JOB_INTERFACE_H_

#include <sup/oac-tree/job_state_monitor.h>
#include <sup/oac-tree/user_interface.h>

namespace sup
{
namespace oac_tree
{
class JobInterface : public UserInterface, public JobStateMonitor
{
public:
  virtual ~JobInterface();
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_JOB_INTERFACE_H_
