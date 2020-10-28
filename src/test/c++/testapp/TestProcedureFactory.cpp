#include "TestProcedureFactory.h"

#include "SuccessNode.h"
#include "instructions/Inverter.h"

TestProcedureFactory::TestProcedureFactory() = default;

TestProcedureFactory::~TestProcedureFactory() = default;

std::unique_ptr<Procedure> TestProcedureFactory::CreateProcedure() const
{
    auto result = std::unique_ptr<Procedure>(new Procedure());

    result->PushInstruction(new SuccessNode());
    result->PushInstruction(new SuccessNode());
    auto inverter = new Inverter();
    inverter->SetInstruction(new SuccessNode());
    result->PushInstruction(inverter);

    return result;
}
