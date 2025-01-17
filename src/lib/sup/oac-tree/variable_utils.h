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

#ifndef SUP_OAC_TREE_VARIABLE_UTILS_H_
#define SUP_OAC_TREE_VARIABLE_UTILS_H_

#include <sup/oac-tree/variable_info.h>
#include <sup/oac-tree/workspace_info.h>

#include <sup/dto/anyvalue.h>

namespace sup
{
namespace oac_tree
{
class Workspace;
class Variable;

namespace utils
{

/**
 * @brief Create a representation of all variables in a workspace, providing their names, types and
 * attributes.
 *
 * @param ws Workspace to use.
 *
 * @return WorkspaceInfo object representing all variables in the given workspace.
 */
WorkspaceInfo CreateWorkspaceInfo(const Workspace& ws);

/**
 * @brief Convert the given AnyValue to a list of VariableInfo objects that represent a workspace.
 *
 * @param ws_info_anyvalue AnyValue representation of all workspace variables.
 *
 * @return WorkspaceInfo object.
 * @throw InvalidOperationException when the provided anyvalue has the wrong format.
 */
WorkspaceInfo ToWorkspaceInfo(const sup::dto::AnyValue& ws_info_anyvalue);

/**
 * @brief Convert the given WorkspaceInfo object to an AnyValue.
 *
 * @param ws_info WorkspaceInfo object.
 *
 * @return AnyValue representation of the WorkspaceInfo object.
 */
sup::dto::AnyValue ToAnyValue(const WorkspaceInfo& ws_info);

/**
 * @brief Validate if the given AnyValue has the right format to be parsed as a WorkspaceInfo
 * object.
 *
 * @details This function only checks that the AnyValue is a structure and each member can be
 * parsed as a VariableInfo object.
 *
 * @param ws_info AnyValue to validate.
 *
 * @return true when the provided AnyValue can be correctly parsed to a WorkspaceInfo object.
 */
bool ValidateWorkspaceInfoAnyValue(const sup::dto::AnyValue& ws_info);

/**
 * @brief Build an list of variable names from the WorkspaceInfo object, ordered by
 * index. This allows O(1) lookup. It is assumed that all indices are unique and span exactly the
 * range from zero to (number_of_variables -1).
 *
 * @param ws_info WorkspaceInfo object.
 *
 * @return List of variable names, ordered by index.
 */
std::vector<std::string> BuildVariableNameMap(const WorkspaceInfo& ws_info);

/**
 * @brief Create a VariableInfo representation of a variable, providing its type,
 * attributes and the variable index used for publishing its status.
 *
 * @param var Variable to represent.
 * @param index Index to put inside the object (referring to the served variable AnyValue status).
 *
 * @return VariableInfo representation.
 * @throw InvalidOperationException when a nullptr is passed.
 */
VariableInfo CreateVariableInfo(const Variable* var, sup::dto::uint32 index);

/**
 * @brief Convert the given AnyValue to a VariableInfo object.
 *
 * @param var_info_anyvalue AnyValue representation of a variable.
 *
 * @return VariableInfo object.
 * @throw InvalidOperationException when the provided anyvalue has the wrong format.
 */
VariableInfo ToVariableInfo(const sup::dto::AnyValue& var_info_anyvalue);

/**
 * @brief Convert the given VariableInfo object to an AnyValue.
 *
 * @param var_info VariableInfo object.
 *
 * @return AnyValue representation of the VariableInfo object.
 */
sup::dto::AnyValue ToAnyValue(const VariableInfo& var_info);

/**
 * @brief Validate if the given AnyValue has the right format to be parsed as a VariableInfo
 * object.
 *
 * @details This function only checks that the AnyValue is a structure and contains the mandatory
 * member fields with the correct type. Any extra fields in the structure are ignored.
 *
 * @param var_info AnyValue to validate.
 *
 * @return true when the provided AnyValue can be correctly parsed to a VariableInfo object.
 */
bool ValidateVariableInfoAnyValue(const sup::dto::AnyValue& var_info);

}  // namespace utils

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_VARIABLE_UTILS_H_
