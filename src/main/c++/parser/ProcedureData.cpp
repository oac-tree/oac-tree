#include "ProcedureData.h"

ProcedureData::ProcedureData(InstructionData * root, WorkspaceData * ws_data)
  : __root{root}
  , __workspace{ws_data}
{}

std::unique_ptr<Procedure> ProcedureData::CreateProcedure() const
{
  return {};
}
