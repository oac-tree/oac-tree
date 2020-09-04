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

void Runner::ExecuteProcedure()
{
    if (__proc)
    {
        while(!IsFinished())
        {
            ExecuteSingle();
        }
    }
}

void Runner::ExecuteSingle()
{
    if (__proc)
    {
        __proc->ExecuteSingle(__ui);
    }
}

bool Runner::IsFinished() const
{
    if (!__proc)
    {
        return true;
    }

    auto status = __proc->GetStatus();
    return (status == ExecutionStatus::SUCCESS ||
            status == ExecutionStatus::FAILURE);
}
