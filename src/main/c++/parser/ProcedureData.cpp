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

// Global header files

#include <common/log-api.h>

// Local header files

#include "ProcedureData.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

ProcedureData::ProcedureData(InstructionData * root, WorkspaceData * ws_data)
  : _root{root}
  , _workspace{ws_data}
{}

std::unique_ptr<Procedure> ProcedureData::CreateProcedure() const
{
  log_info("sup::sequencer::ProcedureData::CreateProcedure() - entering function..");
  auto result = std::unique_ptr<Procedure>(new Procedure());
  if (_root && _root->GetType() == "Sequence")
  {
    result->SetRootInstruction(_root->GenerateInstruction().release());
  }
  if (_workspace)
  {
    log_info("sup::sequencer::ProcedureData::CreateProcedure() - generating workspace variables..");
    for (const auto & var_data : _workspace->GetVariableDataList())
    {
      auto name = var_data.GetName();
      if (!name.empty())
      {
        auto var = var_data.GenerateVariable();
        result->AddVariable(name, var.release());
      }
    }
  }
  return result;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
