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

#include "ProcedureParser.h"
#include "InstructionParser.h"
#include "VariableParser.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

static const std::string WORKSPACE_TYPE = "Workspace";

// Function declaration

// Function definition

std::unique_ptr<Procedure> ParseProcedure(const TreeData & data)
{
  log_info("sup::sequencer::ParseProcedure() - entering function..");
  auto result = std::unique_ptr<Procedure>(new Procedure());

  for (const auto &child : data.Children())
  {
    if (child.GetType() == WORKSPACE_TYPE)
    {
      log_info("sup::sequencer::ParseProcedure() - generating workspace variables..");
      for (const auto &var_data : child.Children())
      {
        auto name = var_data.GetName();
        log_info("sup::sequencer::ParseProcedure() - generate variable: '%s'", name.c_str());
        if (!name.empty())
        {
          auto var = ParseVariable(var_data);
          result->AddVariable(name, var.release());
        }
      }
    }
    // Every non workspace element of the Procedure node should be an instruction node
    else
    {
      auto root_instr = ParseInstruction(child);
      if (root_instr)
      {
        result->SetRootInstruction(root_instr.release());
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
