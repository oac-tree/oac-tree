#include "Sequence.h"


ExecutionStatus Sequence::ExecuteSingleImpl(Runner * runner)
{
    if (__children.empty())
    {
        return ExecutionStatus::SUCCESS;
    }
    for (auto instruction : __children)
    {
        auto child_status = instruction->GetStatus();

        if (child_status == ExecutionStatus::SUCCESS)
        {
            continue;
        }

        if (child_status == ExecutionStatus::UNDEFINED ||
            child_status == ExecutionStatus::STARTED)
        {
            instruction->ExecuteSingle(runner);

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
    for (auto instruction : __children)
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
{}

Sequence::~Sequence()
{}
