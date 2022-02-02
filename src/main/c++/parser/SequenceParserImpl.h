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
 * @file SequenceParserImpl.h
 * @brief Header file for SequenceParserImpl class.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the SequenceParserImpl class.
 */

#ifndef _SEQ_SequenceParserImpl_h_
#define _SEQ_SequenceParserImpl_h_

// Global header files

#include <memory>

// Local header files

#include "TreeData.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
// Global variables

// Function declarations

std::unique_ptr<TreeData> ParseXMLDataFile(const std::string& filename);

std::unique_ptr<TreeData> ParseXMLDataString(const std::string& xml_str);

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

#endif  // _SEQ_SequenceParserImpl_h_
