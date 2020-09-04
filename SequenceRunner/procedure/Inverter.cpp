#include "Inverter.h"


ExecutionStatus Inverter::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
    if (!__child)
    {
        return ExecutionStatus::SUCCESS;
    }

    auto child_status = __child->GetStatus();
    if (child_status == ExecutionStatus::UNDEFINED ||
        child_status == ExecutionStatus::STARTED)
    {
        __child->ExecuteSingle(ui, ws);
    }

    return CalculateStatus();
}

ExecutionStatus Inverter::CalculateStatus() const
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
        break;
    case ExecutionStatus::SUCCESS:
        status = ExecutionStatus::FAILURE;
        break;
    default:
        break;
    }
    return status;
}

Inverter::Inverter()
{}

Inverter::~Inverter()
{}
