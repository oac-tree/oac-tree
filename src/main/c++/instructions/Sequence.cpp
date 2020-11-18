#include "Sequence.h"
#include "InstructionRegistry.h"

ExecutionStatus Sequence::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
    if (_children.empty())
    {
        return ExecutionStatus::SUCCESS;
    }
    for (auto instruction : _children)
    {
        auto child_status = instruction->GetStatus();

        if (child_status == ExecutionStatus::SUCCESS)
        {
            continue;
        }

        if (child_status == ExecutionStatus::UNDEFINED ||
            child_status == ExecutionStatus::STARTED)
        {
            instruction->ExecuteSingle(ui, ws);

            break;
        }
        else
        {
            return child_status; // This should not happen: set error message
        }
    }
    return CalculateCompoundStatus();
}

ExecutionStatus Sequence::CalculateCompoundStatus() const
{
    for (auto instruction : _children)
    {
        auto child_status = instruction->GetStatus();

        if (child_status == ExecutionStatus::SUCCESS)
        {
            continue;
        }

        if (child_status == ExecutionStatus::UNDEFINED ||
            child_status == ExecutionStatus::STARTED)
        {
            return GetStatus();
        }
        else
        {
            return child_status;
        }
    }
    return ExecutionStatus::SUCCESS;
}

Sequence::Sequence()
    : CompoundInstruction(InstructionRegistry::SequenceType)
{}

Sequence::~Sequence()
{}
