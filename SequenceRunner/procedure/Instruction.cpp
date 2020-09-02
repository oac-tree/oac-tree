#include "Instruction.h"

#include "runner/Runner.h"

void Instruction::Preamble(Runner * runner)
{
    __status = ExecutionStatus::STARTING;
    runner->UpdateInstructionStatus(this, __status);
}

void Instruction::Postamble(Runner * runner)
{
    runner->UpdateInstructionStatus(this, __status);
}

Instruction::Instruction()
    : __status{ExecutionStatus::UNDEFINED}
{}

Instruction::~Instruction() = default;

void Instruction::Execute(Runner * runner)
{
    Preamble(runner);

    __status = ExecuteImpl(runner);

    Postamble(runner);
}

void Instruction::ResetStatus()
{
    __status = ExecutionStatus::UNDEFINED;
}
