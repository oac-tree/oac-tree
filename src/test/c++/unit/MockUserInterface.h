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
 * @file MockUserInterface.h
 * @brief Header file for MockUserInterface class.
 * @date 16/03/21
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the MockUserInterface class.
 */

#ifndef _SEQ_MockUserInterface_h_
#define _SEQ_MockUserInterface_h_

// Global header files

#include <gmock/gmock.h>
#include <cstring>

// Local header files

#include "Instruction.h"
#include "UserInterface.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
/**
 * @brief Mock UserInterface class for unit testing.
 */
class MockUserInterface : public UserInterface
{
public:
  MOCK_METHOD1(UpdateInstructionStatusImpl, void(const Instruction *));
  MOCK_METHOD2(VariableUpdatedImpl,
               void( const std::string &name, const ::ccs::types::AnyValue &value));
  MOCK_METHOD2(PutValueImpl,
               bool(const ::ccs::types::AnyValue &value, const std::string &description));
  MOCK_METHOD2(GetUserValueImpl,
               bool(::ccs::types::AnyValue &value, const std::string &description));
  MOCK_METHOD2(GetUserChoiceImpl,
               int(const std::vector<std::string> &choices, const std::string &description));
  MOCK_METHOD0(StartSingleStepImpl, void());
  MOCK_METHOD0(EndSingleStepImpl, void());
};

MATCHER_P(HasExecutionStatus, expected, "")
{
  return arg->GetStatus() == expected;
}

MATCHER_P(HasSameValue, value, "")
{
  if (arg.GetSize() != value.GetSize())
  {
    return false;
  }
  return (std::memcmp(arg.GetInstance(), value.GetInstance(), arg.GetSize()) == 0);
}

// Global variables

// Function declarations

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

#endif  // _SEQ_MockUserInterface_h_
