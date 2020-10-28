#include "ProcedureData.h"

namespace SequenceParser
{

ProcedureData::ProcedureData(InstructionData * root, WorkspaceData * ws_data)
  : __root{root}
  , __workspace{ws_data}
{}

} // namespace SequenceParser