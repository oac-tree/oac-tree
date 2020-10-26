#include "TestProcedureFactory.h"

#include "SuccesNode.h"
#include "instructions/Inverter.h"

TestProcedureFactory::TestProcedureFactory() = default;

TestProcedureFactory::~TestProcedureFactory() = default;

std::unique_ptr<Procedure> TestProcedureFactory::CreateProcedure() const
{
    auto result = std::unique_ptr<Procedure>(new Procedure());

    result->PushInstruction(new SuccesNode());
    result->PushInstruction(new SuccesNode());
    auto inverter = new Inverter();
    inverter->SetInstruction(new SuccesNode());
    result->PushInstruction(inverter);

    return result;
}
