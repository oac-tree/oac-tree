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
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_CL_UTILS_H_
#define SUP_OAC_TREE_CL_UTILS_H_

#include <string>

namespace sup
{
namespace oac_tree
{
const std::string kNoPluginsLoaded = "No plugins loaded";

/**
 * @brief Get a message describing the loaded plugins. Message will indicate if no plugins were
 * loaded.
 *
 * @return Message describing the loaded plugins.
 */
std::string GetLoadedPluginMessage();

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_CL_UTILS_H_
