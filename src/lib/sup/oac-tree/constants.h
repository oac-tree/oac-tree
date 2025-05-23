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

#ifndef SUP_OAC_TREE_CONSTANTS_H_
#define SUP_OAC_TREE_CONSTANTS_H_

#include <sup/dto/anyvalue.h>

#include <string>

namespace sup
{
namespace oac_tree
{
namespace DefaultSettings
{
/**
 * @brief Default sleep time (in ns) between ticks.
 */
const sup::dto::int64 DEFAULT_SLEEP_TIME_NS = 100000000; // 100 ms

/**
 * @brief Default value (in ns) for the maximum time an instruction should block.
 *
 * @note Implementers of instructions are responsible for using this value as a guideline.
 */
const sup::dto::int64 MAX_BLOCKING_TIME_NS = 100000000; // 100 ms

/**
 * @brief Desired accuracy (in ns) for instructions containing time related functions.
 *
 * @note This value should not be considered as a strict requirement but rather as
 * a guideline.
 */
const sup::dto::int64 DEFAULT_TIMING_ACCURACY_NS = 50000000; // 50 ms

/**
 * @brief When used as the first character of an attribute's value, this character
 * indicates the value needs to be initialised from another attribute.
 *
 * @note See the Include instruction for more details.
 */
const char PLACEHOLDER_ATTRIBUTE_CHAR = '$';

/**
 * @brief When used as the first character of an instruction attribute's value, this character
 * indicates the value needs to be fetched from the workspace using the variable name that follows
 * the character.
 *
 * @note Instructions can decide to support this mechanism. If not explicitly enabled, it is not
 * supported.
 */
const char VARIABLE_ATTRIBUTE_CHAR = '@';

/**
 * @brief Character to be used as a separator between different elements of an instruction path.
 *
 * @note See the Include instruction for more details.
 */
const char PATH_DELIMITER = '.';

/**
 * @brief Character to be used as a separator between different variable names in an attribute.
 *
 * @note See the Include instruction for more details.
 */
const char VARNAME_DELIMITER = ',';
}  // namespace DefaultSettings

namespace Constants
{
// Top level XML element and attribute names
const std::string PROCEDURE_ELEMENT_NAME = "Procedure";
const std::string WORKSPACE_ELEMENT_NAME = "Workspace";
const std::string PLUGIN_ELEMENT_NAME = "Plugin";
const std::string REGISTERTYPE_ELEMENT_NAME = "RegisterType";
const std::string REGISTERTYPE_JSON_FILE_ATTRIBUTE = "jsonfile";
const std::string REGISTERTYPE_JSON_TYPE_ATTRIBUTE = "jsontype";

// User choice related constants
const std::string USER_CHOICES_METADATA_NAME = "sup::sequencerUserChoiceMetadata/v1.0";
const std::string USER_CHOICES_TEXT_NAME = "text";
const std::string USER_CHOICES_DIALOG_TYPE_NAME = "dialog_type";
const std::string USER_CHOICES_MODAL_NAME = "modal";
const std::string USER_CHOICES_TITLE_NAME = "title";
const std::string USER_CHOICES_INFORMATIVE_TEXT_NAME = "informative";
const std::string USER_CHOICES_DETAILS_NAME = "details";

// Universal instruction attribute names
const std::string IS_ROOT_ATTRIBUTE_NAME = "isRoot";
const std::string NAME_ATTRIBUTE_NAME = "name";
const std::string SHOW_COLLAPSED_ATTRIBUTE_NAME = "showCollapsed";

// Instruction attribute names
const std::string GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME = "varName";
const std::string INPUT_ATTRIBUTE_NAME = "input";
const std::string INPUT_VARIABLE_NAME_ATTRIBUTE_NAME = "inputVar";
const std::string OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME = "outputVar";
const std::string FILENAME_ATTRIBUTE_NAME = "file";
const std::string PATH_ATTRIBUTE_NAME = "path";
const std::string LEFT_ATTRIBUTE_NAME = "left";
const std::string LEFT_VARIABLE_NAME_ATTRIBUTE_NAME = "leftVar";
const std::string RIGHT_ATTRIBUTE_NAME = "right";
const std::string RIGHT_VARIABLE_NAME_ATTRIBUTE_NAME = "rightVar";
const std::string ARRAY_VARIABLE_NAME_ATTRIBUTE_NAME = "arrayVar";
const std::string ELEMENT_VARIABLE_NAME_ATTRIBUTE_NAME = "elementVar";
const std::string DESCRIPTION_ATTRIBUTE_NAME = "description";
const std::string VARIABLE_NAMES_ATTRIBUTE_NAME = "varNames";
const std::string FORCE_SUCCESS_ATTRIBUTE_NAME = "forceSuccess";
const std::string MESSAGE_ATTRIBUTE_NAME = "message";
const std::string SEVERITY_ATTRIBUTE_NAME = "severity";
const std::string TEXT_ATTRIBUTE_NAME = "text";
const std::string FROM_VARIABLE_ATTRIBUTE_NAME = "fromVar";
const std::string SUCCESS_THRESHOLD_ATTRIBUTE_NAME = "successThreshold";
const std::string FAILURE_THRESHOLD_ATTRIBUTE_NAME = "failureThreshold";
const std::string MAX_COUNT_ATTRIBUTE_NAME = "maxCount";
const std::string OK_TEXT_ATTRIBUTE_NAME = "okText";
const std::string CANCEL_TEXT_ATTRIBUTE_NAME = "cancelText";
const std::string TIMEOUT_SEC_ATTRIBUTE_NAME = "timeout";
const std::string EQUALS_VARIABLE_NAME_ATTRIBUTE_NAME = "equalsVar";
const std::string VARIABLE_TYPE_ATTRIBUTE_NAME = "varType";
const std::string MEMBER_NAME_ATTRIBUTE_NAME = "memberName";
const std::string BLOCKING_ATTRIBUTE_NAME = "blocking";

// Variable attribute names
const std::string TYPE_ATTRIBUTE_NAME = "type";
const std::string VALUE_ATTRIBUTE_NAME = "value";
const std::string IS_DYNAMIC_TYPE_ATTRIBUTE_NAME = "dynamicType";
const std::string PRETTY_JSON_ATTRIBUTE_NAME = "pretty";

// Instruction info node fields
const std::string kInstructionInfoNodeType = "sup::instructionNodeInfoType/v1.0";
const std::string kInstructionInfoNodeTypeField = "type";
const std::string kInstructionInfoNodeCategoryField = "category";
const std::string kIndexField = "index";
const std::string kAttributesField = "attributes";
const std::string kChildInstructionsField = "children";
const std::string kChildMemberFieldPrefix = "child_";

// Basic instruction info node AnyValue
extern const sup::dto::AnyValue kInstructionInfoNodeAnyValue;

// Variable info node fields
const std::string kVariableInfoType = "sup::variableInfoType/v1.0";
const std::string kVariableInfoTypeField = "type";

// Basic variable info node AnyValue
extern const sup::dto::AnyValue kVariableInfoAnyValue;

// Job info node fields:
const std::string kJobInfoType = "sup::jobInfoType/v1.0";
const std::string kFullNameFieldName = "full_name";
const std::string kWorkspaceInfoFieldName = "ws_info";
const std::string kInstructionTreeInfoFieldName = "instr_tree_info";

// Basic job info node AnyValue
extern const sup::dto::AnyValue kJobInfoAnyValue;

// Instruction state fields:
const std::string kInstructionStateType = "sup::instructionStateType/v1.0";
const std::string kExecStatusField = "exec_status";
const std::string kBreakpointField = "breakpoint";

// Basic instruction AnyValue
extern const sup::dto::AnyValue kInstructionStateAnyValue;

// Field names used in the input request AnyValue:
const std::string kInputRequestOptionsFieldName = "options";
const std::string kInputRequestMetadataFieldName = "metadata";

} // namespace Constants

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_CONSTANTS_H_
