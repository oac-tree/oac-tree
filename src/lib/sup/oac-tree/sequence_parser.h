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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_SEQUENCE_PARSER_H_
#define SUP_OAC_TREE_SEQUENCE_PARSER_H_

#include <sup/oac-tree/procedure.h>

#include <memory>
#include <string>

namespace sup
{
namespace oac_tree
{
void LoadPlugin(const std::string& name);

std::unique_ptr<Procedure> ParseProcedureFile(const std::string& filename);

std::unique_ptr<Procedure> ParseProcedureString(const std::string& xml_str);

std::string GetXMLString(const Procedure& procedure);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_SEQUENCE_PARSER_H_
