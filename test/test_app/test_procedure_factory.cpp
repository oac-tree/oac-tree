#include "test_procedure_factory.h"

#include <sup/sequencer/instructions/wait.h>
#include <sup/sequencer/instructions/inverter.h>

using namespace sup::sequencer;

TestProcedureFactory::TestProcedureFactory() = default;

TestProcedureFactory::~TestProcedureFactory() = default;

std::unique_ptr<Procedure> TestProcedureFactory::CreateProcedure() const
{
  auto result = std::unique_ptr<Procedure>(new Procedure());

  result->PushInstruction(new Wait());
  result->PushInstruction(new Wait());
  auto inverter = new Inverter();
  inverter->SetInstruction(new Wait());
  result->PushInstruction(inverter);

  return result;
}
