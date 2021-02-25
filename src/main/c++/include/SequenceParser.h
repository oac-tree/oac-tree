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
 * @file SequenceParser.h
 * @brief Header file for SequenceParser functions.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the SequenceParser functions.
 */

#ifndef _SEQ_SequenceParser_h_
#define _SEQ_SequenceParser_h_

// Global header files

#include <memory>
#include <string>

// Local header files

#include "Procedure.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

// Global variables

// Function declarations

bool LoadPlugin(const std::string & name);

std::unique_ptr<Procedure> ParseProcedureFile(const std::string & filename);

std::unique_ptr<Procedure> ParseProcedureString(const std::string & xml_str);

// Function definitions

} // namespace sequencer

} // namespace sup

extern "C" {
#endif // __cplusplus

// C API function declarations

#ifdef __cplusplus
} // extern C
#endif // __cplusplus

#endif // _SEQ_SequenceParser_h_
