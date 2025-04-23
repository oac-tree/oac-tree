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

#ifndef SUP_OAC_TREE_INSTRUCTION_UTILS_H_
#define SUP_OAC_TREE_INSTRUCTION_UTILS_H_

#include <sup/oac-tree/instruction.h>

#include <sup/dto/basic_scalar_types.h>

#include <string>
#include <vector>

namespace sup
{
namespace oac_tree
{
namespace instruction_utils
{

const double kMaxTimeoutSeconds = 9.2e9; // More than 290 years. This should be enough...

/**
 * @brief Convert a floating point timeout in seconds to integer nanoseconds. Also checks for
 * limits (positive and smaller that kMaxTimeoutSeconds)
 *
 * @param timeout_sec Timeout in seconds.
 * @param timeout_ns Output timeout in nanoseconds.
 *
 * @return True on success, false otherwise.
 */
bool ConvertToTimeoutNanoseconds(sup::dto::float64 timeout_sec, sup::dto::int64& timeout_ns);

/**
 * @brief Retrieve a timeout in nanoseconds from a variable attribute that encodes a floating point
 * timeout in seconds.
 *
 * @param instr Instruction to query.
 * @param ui UserInterface to use for logging warnings/errors.
 * @param ws Workspace to use if value needs to be fetched from a variable.
 * @param attr_name Attribute name.
 * @param timeout_ns Output timeout in nanoseconds.
 *
 * @return True on success, false otherwise.
 */
bool GetVariableTimeoutAttribute(const Instruction& instr, UserInterface& ui, Workspace& ws,
                                 const std::string& attr_name, sup::dto::int64& timeout_ns);

/**
 * @brief Get a list of variable names from an instruction attribute.
 *
 * @param instr Instruction that holds the attribute.
 * @param attr_name Name of attribute that holds a list of variable names.
 *
 * @return List of variable names.
 *
 * @throws InstructionSetupException when the attribute is not present.
 *
 * @note Variable names should be separated by VARNAME_DELIMITER, defined in constants.h.
 */
std::vector<std::string> VariableNamesFromAttribute(const Instruction& instr,
                                                    const std::string& attr_name);

}  // namespace instruction_utils

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_INSTRUCTION_UTILS_H_
