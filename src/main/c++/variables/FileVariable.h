/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project       : CODAC Supervision and Automation (SUP) Sequencer component
*
* Description   : File-based variable implementation
*
* Author        : Bertrand Bauvir (IO)
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
 * @file FileVariable.h
 * @brief Header file for FileVariable class.
 * @date 11/12/2020
 * @author Bertrand Bauvir (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the FileVariable class.
 */

#ifndef _SEQ_FileVariable_h_
#define _SEQ_FileVariable_h_

// Global header files

// Local header files

#include "Variable.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief FileVariable class.
 * @details Variable with file-based backend.
 */
class FileVariable : public Variable
{
  private:
    /**
     * @brief See sup::sequencer::Variable.
     */
    bool GetValueImpl(::ccs::types::AnyValue& value) const override;
    bool SetValueImpl(const ::ccs::types::AnyValue& value) override;
    bool SetupImpl() override;
  protected:

  public:
    /**
     * @brief Constructor.
     */
    FileVariable ();

    /**
     * @brief Destructor.
     */
    ~FileVariable () override;

    /**
     * @brief Class name for VariableRegistry.
     */
    static const std::string Type;
};

// Global variables

// Function declaration

// Function definition

} // namespace sequencer

} // namespace sup

extern "C" {
#endif // __cplusplus

// C API function declarations

#ifdef __cplusplus
} // extern C
#endif // __cplusplus

#endif // _SEQ_FileVariable_h_