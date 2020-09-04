#include "Instruction.h"

#include "runner/Runner.h"

void Instruction::Preamble(Runner * runner)
{
    if (__status == ExecutionStatus::UNDEFINED)
    {
        __status = ExecutionStatus::STARTED;
        runner->UpdateInstructionStatus(this);
    }
}

void Instruction::Postamble(Runner * runner)
{
    if (__status != __status_before)
    {
        runner->UpdateInstructionStatus(this);
    }
}

Instruction::Instruction()
    : __status{ExecutionStatus::UNDEFINED}
    , __status_before{ExecutionStatus::UNDEFINED}
{}

Instruction::~Instruction() = default;

void Instruction::ExecuteSingle(Runner * runner)
{
    Preamble(runner);

    __status_before = __status;

    __status = ExecuteSingleImpl(runner);

    Postamble(runner);
}

ExecutionStatus Instruction::GetStatus() const
{
    return __status;
}

void Instruction::ResetStatus()
{
    __status = ExecutionStatus::UNDEFINED;
}
