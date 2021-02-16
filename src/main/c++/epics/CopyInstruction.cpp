/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project       : CODAC Supervision and Automation (SUP) Sequencer component
*
* Description   : Instruction node implementation
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

// Global header files

#include <new> // std::nothrow, etc.

// Local header files

#include "Instruction.h"
#include "InstructionRegistry.h"

#include "Workspace.h"
#include "Variable.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

/**
 * @brief Xxx
 */

class CopyInstruction : public Instruction
{

  private:

    /**
     * @brief Xxx
     */

    ExecutionStatus ExecuteSingleImpl (UserInterface * ui, Workspace * ws) override;

  protected:

  public:

    /**
     * @brief Constructor.
     */

    CopyInstruction (void);

    /**
     * @brief Destructor.
     */

    ~CopyInstruction (void) override;

};

// Function declaration

bool RegisterCopyInstruction (void);

// Global variables

static ccs::log::Func_t _handler = ccs::log::SetStdout();

static bool global_copyinstruction_initialised_flag = RegisterCopyInstruction();

// Function definition

bool RegisterCopyInstruction (void)
{

  auto constructor = []() { return static_cast<Instruction*>(new CopyInstruction ()); };
  GlobalInstructionRegistry().RegisterInstruction("CopyInstruction", constructor);

  return true;

}

ExecutionStatus CopyInstruction::ExecuteSingleImpl (UserInterface * ui, Workspace * ws)
{

  (void)ui;
  (void)ws;

  bool status = (Instruction::HasAttribute("input") && Instruction::HasAttribute("output"));

  ccs::types::AnyValue _value;

  if (status)
    { // Read from workspace
      status = ws->GetValue(GetAttribute("input"), _value);
    }

  if (status)
    {
      status = ws->SetValue(GetAttribute("output"), _value);
    }

  return (status ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE);

}

CopyInstruction::CopyInstruction (void) : Instruction("CopyInstruction") {}
CopyInstruction::~CopyInstruction (void) {}

} // namespace sequencer

} // namespace sup

#undef LOG_ALTERN_SRC
