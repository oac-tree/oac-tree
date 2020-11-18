#include "Inverter.h"
#include "InstructionRegistry.h"


ExecutionStatus Inverter::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
    if (!_child)
    {
        return ExecutionStatus::SUCCESS;
    }

    auto child_status = _child->GetStatus();
    if (child_status == ExecutionStatus::UNDEFINED ||
        child_status == ExecutionStatus::STARTED)
    {
        _child->ExecuteSingle(ui, ws);
    }

    return CalculateStatus();
}

ExecutionStatus Inverter::CalculateStatus() const
{
    if (!_child)
    {
        return ExecutionStatus::SUCCESS;
    }

    auto child_status = _child->GetStatus();
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
    : DecoratorInstruction(InstructionRegistry::InverterType)
{}

Inverter::~Inverter()
{}
