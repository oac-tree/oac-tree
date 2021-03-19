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
* Copyright (c) : 2010-2020 ITER Organization,
*                 CS 90 046
*                 13067 St. Paul-lez-Durance Cedex
*                 France
*
* This file is part of ITER CODAC software.
* For the terms and conditions of redistribution or use of this software
* refer to the file ITER-LICENSE.TXT located in the top level directory
* of the distribution package.
******************************************************************************/

/**
 * @file Constants.h
 * @brief Header file for globally defined constants and default settings.
 * @date 16/03/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of globally defined constants and default settings.
 */

#ifndef _SEQ_Constants_h_
#define _SEQ_Constants_h_

// Global header files

// Local header files

// Constants

#ifdef __cplusplus

namespace sup {

namespace sequencer {

// Forward declarations

// Type definition

// Global variables

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
static const char VAR_ATTRIBUTE_CHAR='$';

/**
 * @brief Character to be used a separator between different elements of an instruction path.
 *
 * @note See the Include instruction for more details.
 */
static const char PATH_DELIMITER='.';
}

// Function declarations

// Function definitions

} // namespace sequencer

} // namespace sup

extern "C" {
#endif // __cplusplus

// C API function declarations

#ifdef __cplusplus
} // extern C
#endif // __cplusplus

#endif // _SEQ_Constants_h_
