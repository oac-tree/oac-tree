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

#ifndef SUP_SEQUENCER_CONSTANTS_H_
#define SUP_SEQUENCER_CONSTANTS_H_

#include <string>

namespace sup
{
namespace sequencer
{
namespace DefaultSettings
{
/**
 * @brief Default sleep time (in ms) between ticks.
 */
const int DEFAULT_SLEEP_TIME_MS = 100;

/**
 * @brief Default value (in ms) for the maximum time an instruction should block.
 *
 * @note Implementers of instructions are responsible for using this value as a guideline.
 */
const int MAX_BLOCKING_TIME_MS = 100;

/**
 * @brief Desired accuracy (in ms) for instructions containing time related functions.
 *
 * @note This value should not be considered as a strict requirement but rather as
 * a guideline.
 */
const int TIMING_ACCURACY_MS = 50;

/**
 * @brief When used as the first character of an attribute's value, this character
 * indicates the value needs to be initialised from another attribute.
 *
 * @note See the Include instruction for more details.
 */
const char VAR_ATTRIBUTE_CHAR = '$';

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
// XML element and attribute names
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

} // namespace Constants

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_CONSTANTS_H_
