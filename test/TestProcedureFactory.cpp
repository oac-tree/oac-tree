#include "TestProcedureFactory.h"

#include "SuccesNode.h"

TestProcedureFactory::TestProcedureFactory() = default;

TestProcedureFactory::~TestProcedureFactory() = default;

std::unique_ptr<Procedure> TestProcedureFactory::CreateProcedure() const
{
    auto result = std::unique_ptr<Procedure>(new Procedure());

    result->PushInstruction(new SuccesNode());

    return result;
}
