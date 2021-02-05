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
 * @file ProcedureParser.h
 * @brief Header file for ProcedureParser functions.
 * @date 04/02/21
 * @author Walter Van Herck (IO)
 * @copyright 2010-2021 ITER Organization
 * @details This header file contains the definition of the ProcedureParser functions.
 */

#ifndef _SEQ_ProcedureParser_h_
#define _SEQ_ProcedureParser_h_

// Global header files

#include <memory>

// Local header files

#include "TreeData.h"
#include "Procedure.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

// Global variables

// Function declarations

std::unique_ptr<Procedure> ParseProcedure(const TreeData & data);

// Function definitions

} // namespace sequencer

} // namespace sup

extern "C" {
#endif // __cplusplus

// C API function declarations

#ifdef __cplusplus
} // extern C
#endif // __cplusplus

#endif // _SEQ_ProcedureParser_h_
