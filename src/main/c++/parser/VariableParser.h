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
 * Copyright (c) : 2010-2022 ITER Organization,
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
 * @file VariableParser.h
 * @brief Header file for the VariableParser functions.
 * @date 04/02/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the VariableParser functions.
 */

#ifndef _SEQ_VariableParser_h_
#define _SEQ_VariableParser_h_

// Global header files

#include <memory>
#include <string>

// Local header files

#include "TreeData.h"
#include "Variable.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
// Global variables

// Function declarations

/**
 * @brief Parse to Variable.
 */
std::unique_ptr<Variable> ParseVariable(const TreeData& data);

// Function definitions

}  // namespace sequencer

}  // namespace sup

extern "C"
{
#endif  // __cplusplus

  // C API function declarations

#ifdef __cplusplus
}  // extern C
#endif  // __cplusplus

#endif  // _SEQ_VariableParser_h_
