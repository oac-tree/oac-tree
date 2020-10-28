#ifndef PROCEDUREDATA_H
#define PROCEDUREDATA_H

#include <memory>

#include "InstructionData.h"
#include "WorkspaceData.h"

namespace SequenceParser
{

class ProcedureData
{
private:
  std::unique_ptr<InstructionData> __root;
  std::unique_ptr<WorkspaceData> __workspace;

public:
  ProcedureData(InstructionData * root, WorkspaceData * ws_data);

  ~ProcedureData() = default;
};

} // namespace SequenceParser

#endif // PROCEDUREDATA_H