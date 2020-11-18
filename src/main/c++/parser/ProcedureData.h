#ifndef PROCEDUREDATA_H
#define PROCEDUREDATA_H

#include <memory>

#include "InstructionData.h"
#include "WorkspaceData.h"
#include "procedure/Procedure.h"

class ProcedureData
{
private:
  std::unique_ptr<InstructionData> _root;
  std::unique_ptr<WorkspaceData> _workspace;

public:
  ProcedureData(InstructionData * root, WorkspaceData * ws_data);

  ~ProcedureData() = default;

  std::unique_ptr<Procedure> CreateProcedure() const;
};

#endif // PROCEDUREDATA_H