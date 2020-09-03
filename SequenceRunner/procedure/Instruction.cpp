#include "Instruction.h"

#include "runner/Runner.h"

void Instruction::Preamble(Runner * runner)
{
    if (__status == ExecutionStatus::UNDEFINED)
    {
        __status = ExecutionStatus::STARTED;
        runner->UpdateInstructionStatus(this, __status);
    }
}

void Instruction::Postamble(Runner * runner)
{
    if (__status != __status_before)
    {
        runner->UpdateInstructionStatus(this, __status);
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

void Instruction::ResetStatus()
{
    __status = ExecutionStatus::UNDEFINED;
}
