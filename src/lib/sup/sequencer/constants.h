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
static const int DEFAULT_SLEEP_TIME_MS = 100;

/**
 * @brief Default value (in ms) for the maximum time an instruction should block.
 *
 * @note Implementers of instructions are responsible for using this value as a guideline.
 */
static const int MAX_BLOCKING_TIME_MS = 100;

/**
 * @brief Desired accuracy (in ms) for instructions containing time related functions.
 *
 * @note This value should not be considered as a strict requirement but rather as
 * a guideline.
 */
static const int TIMING_ACCURACY_MS = 50;

/**
 * @brief When used as the first character of an attribute's value, this character
 * indicates the value needs to be initialised from another attribute.
 *
 * @note See the Include instruction for more details.
 */
static const char VAR_ATTRIBUTE_CHAR = '$';

/**
 * @brief Character to be used as a separator between different elements of an instruction path.
 *
 * @note See the Include instruction for more details.
 */
static const char PATH_DELIMITER = '.';

/**
 * @brief Character to be used as a separator between different variable names in an attribute.
 *
 * @note See the Include instruction for more details.
 */
static const char VARNAME_DELIMITER = ',';
}  // namespace DefaultSettings

namespace Constants
{
static const std::string PROCEDURE_ELEMENT_NAME = "Procedure";
static const std::string WORKSPACE_ELEMENT_NAME = "Workspace";
static const std::string PLUGIN_ELEMENT_NAME = "Plugin";
static const std::string REGISTERTYPE_ELEMENT_NAME = "RegisterType";

} // namespace Constants

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_CONSTANTS_H_
