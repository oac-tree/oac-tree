#include "Instruction.h"

#include "runner/UserInterface.h"

void Instruction::Preamble(UserInterface * ui)
{
    if (__status == ExecutionStatus::UNDEFINED)
    {
        __status = ExecutionStatus::STARTED;
        ui->UpdateInstructionStatus(this);
    }
}

void Instruction::Postamble(UserInterface * ui)
{
    if (__status != __status_before)
    {
        ui->UpdateInstructionStatus(this);
    }
}

Instruction::Instruction(std::string type)
    : __type{std::move(type)}
    , __name{}
    , __status{ExecutionStatus::UNDEFINED}
    , __status_before{ExecutionStatus::UNDEFINED}
{}

Instruction::~Instruction() = default;

std::string Instruction::GetType() const
{
    return __type;
}

std::string Instruction::GetName() const
{
    return __name;
}

void Instruction::SetName(std::string name)
{
    __name = std::move(name);
}

void Instruction::ExecuteSingle(UserInterface * ui, Workspace * ws)
{
    Preamble(ui);

    __status_before = __status;

    __status = ExecuteSingleImpl(ui, ws);

    Postamble(ui);
}

ExecutionStatus Instruction::GetStatus() const
{
    return __status;
}

void Instruction::ResetStatus()
{
    __status = ExecutionStatus::UNDEFINED;
}
