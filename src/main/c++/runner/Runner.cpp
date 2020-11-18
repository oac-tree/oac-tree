#include "Runner.h"

Runner::Runner(UserInterface * ui)
    : _proc{nullptr}
    , _ui{ui}
{}

Runner::~Runner()
{}

void Runner::SetProcedure(Procedure * procedure)
{
    _proc = procedure;
}

void Runner::ExecuteProcedure()
{
    if (_proc)
    {
        while(!IsFinished())
        {
            ExecuteSingle();
        }
    }
}

void Runner::ExecuteSingle()
{
    if (_proc)
    {
        _ui->StartSingleStep();
        _proc->ExecuteSingle(_ui);
        _ui->EndSingleStep();
    }
}

bool Runner::IsFinished() const
{
    if (!_proc)
    {
        return true;
    }

    auto status = _proc->GetStatus();
    return (status == ExecutionStatus::SUCCESS ||
            status == ExecutionStatus::FAILURE);
}
