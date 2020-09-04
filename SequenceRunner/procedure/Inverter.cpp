#include "Inverter.h"


ExecutionStatus Invertor::ExecuteSingleImpl(Runner *runner)
{
    if (!__child)
    {
        return ExecutionStatus::SUCCESS;
    }

    auto child_status = __child->GetStatus();
    if (child_status == ExecutionStatus::UNDEFINED ||
        child_status == ExecutionStatus::STARTED)
    {
        __child->ExecuteSingle(runner);
    }

    return CalculateStatus();
}

ExecutionStatus Invertor::CalculateStatus() const
{
    if (!__child)
    {
        return ExecutionStatus::SUCCESS;
    }

    auto child_status = __child->GetStatus();
    auto status = child_status;

    switch (child_status)
    {
    case ExecutionStatus::FAILURE:
        status = ExecutionStatus::SUCCESS;
    case ExecutionStatus::SUCCESS:
        status = ExecutionStatus::FAILURE;
    default:
        break;
    }
    return status;
}

Invertor::Invertor()
{}

Invertor::~Invertor()
{}
