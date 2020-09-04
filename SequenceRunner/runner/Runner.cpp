#include "Runner.h"

Runner::Runner(UserInterface * ui)
    : __proc{nullptr}
    , __ui{ui}
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
        __proc->ExecuteSingle(__ui);
    }
}

void Runner::UpdateInstructionStatus(const Instruction * instruction)
{
    auto status = instruction->GetStatus();
}
