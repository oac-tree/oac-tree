#include "ProcedureData.h"

ProcedureData::ProcedureData(InstructionData * root, WorkspaceData * ws_data)
  : _root{root}
  , _workspace{ws_data}
{}

std::unique_ptr<Procedure> ProcedureData::CreateProcedure() const
{
  return {};
}
