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
* Copyright (c) : 2010-2021 ITER Organization,
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
 * @file UnitTestHelper.h
 * @brief Header file for unit test helper functions.
 * @date 23/03/21
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the unit test helper functions.
 */

#ifndef _SEQ_UnitTestHelper_h_
#define _SEQ_UnitTestHelper_h_

// Global header files

#include <string>

// Local header files

#include "ExecutionStatus.h"
#include "Instruction.h"
#include "Procedure.h"
#include "UserInterface.h"
#include "Workspace.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace UnitTestHelper {

/**
 * @brief Obvious.
 */

class CounterInstruction : public sup::sequencer::Instruction
{

  private:

    /**
     * @brief See sup::sequencer::Instruction.
     */

    sup::sequencer::ExecutionStatus ExecuteSingleImpl (sup::sequencer::UserInterface * ui, sup::sequencer::Workspace * ws) override;

  protected:

  public:

    /**
     * @brief Constructor.
     */

    CounterInstruction (void);

    /**
     * @brief Destructor.
     */

    ~CounterInstruction (void) override;

    /**
     * @brief Class name for InstructionRegistry.
     */

    static const std::string Type;

    static ccs::types::uint32 counter;
    static ccs::types::uint32 GetCount (void);

};

/**
 * @brief Obvious.
 */

class MockUI : public sup::sequencer::UserInterface
{

  private:

    bool _status = false;
    int _choice = -1;
    ::ccs::types::AnyValue _value;
    ::ccs::base::SharedReference<const ::ccs::types::AnyType> _type;

    /**
     * @brief See sup::sequencer::UserInterface.
     */

    virtual void UpdateInstructionStatusImpl (const sup::sequencer::Instruction * instruction);
    virtual bool GetUserValueImpl (::ccs::types::AnyValue & value, const std::string & description);
    virtual int GetUserChoiceImpl (const std::vector<std::string> & choices, const std::string & description);

  protected:

  public:

    ::ccs::base::SharedReference<const ::ccs::types::AnyType> GetType (void) const;

    void SetChoice (int choice);
    void SetStatus (bool status);
    void SetValue (::ccs::types::AnyValue & value);

    MockUI (void);
    virtual ~MockUI (void);

};

class TemporaryTestFile
{
  private:
    std::string filename;

  public:
    TemporaryTestFile(std::string filename, std::string contents);
    ~TemporaryTestFile();
};

// Global variables

// Function declarations

std::string GetFullTestFilePath(const std::string & filename);
static inline bool TryAndExecute (std::unique_ptr<sup::sequencer::Procedure>& proc, sup::sequencer::UserInterface * const ui, const sup::sequencer::ExecutionStatus& expect = sup::sequencer::ExecutionStatus::SUCCESS);

// Function definitions

static inline bool TryAndExecute (std::unique_ptr<sup::sequencer::Procedure>& proc, sup::sequencer::UserInterface * const ui, const sup::sequencer::ExecutionStatus& expect)
{

  bool status = static_cast<bool>(proc);

  if (status)
    { // Setup procedure
      status = proc->Setup();
    }

  if (status)
    {
      sup::sequencer::ExecutionStatus exec = sup::sequencer::ExecutionStatus::FAILURE;

      do
        {
          (void)ccs::HelperTools::SleepFor(100000000ul); // Let system breathe
          proc->ExecuteSingle(ui);
          exec = proc->GetStatus();
        }
      while ((sup::sequencer::ExecutionStatus::SUCCESS != exec) &&
             (sup::sequencer::ExecutionStatus::FAILURE != exec));

      status = (expect == exec);

      proc->Reset(); // Wait for thread termination before calling destructor of UI
    }

  return status;

}

/**
 * Creates a string representing a valid XML of sequencer procedure by enclosing user provided body
 * between appropriate header and footer.
 */
std::string CreateProcedureString(const std::string& body);

/**
 * Prints variables in a workspace.
 */
void PrintProcedureWorkspace(::sup::sequencer::Procedure *procedure);


} // namespace UnitTestHelper

} // namespace sup

extern "C" {
#endif // __cplusplus

// C API function declarations

#ifdef __cplusplus
} // extern C
#endif // __cplusplus

#endif // _SEQ_UnitTestHelper_h_
