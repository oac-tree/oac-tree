#include "Runner.h"

Runner::Runner()
    : __proc{nullptr}
{}

Runner::~Runner()
{}

void Runner::SetProcedure(Procedure * procedure)
{
    __proc = procedure;
}

void Runner::ExecuteSingle()
{
    if (__proc)
    {
        __proc->ExecuteSingle(this);
    }
}

void Runner::UpdateInstructionStatus(const Instruction * instruction)
{
    auto status = instruction->GetStatus();
}
