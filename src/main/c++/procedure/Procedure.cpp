#include "Procedure.h"

Procedure::Procedure()
    : __root_sequence{new Sequence()}
    , __workspace{}
{}

Procedure::~Procedure() = default;

bool Procedure::AddVariable(Variable var)
{
    return __workspace->AddVariable(var);
}

std::vector<std::string> Procedure::VariableNames() const
{
    return __workspace->VariableNames();
}

int Procedure::GetVariableValue(std::string name)
{
    return __workspace->GetVariableValue(name);
}

bool Procedure::SetVariableValue(std::string name, int value)
{
    return __workspace->SetVariableValue(name, value);
}

bool Procedure::PushInstruction(Instruction * instruction)
{
    __root_sequence->PushBack(instruction);
    return true;
}

void Procedure::ExecuteSingle(UserInterface * ui)
{
    __root_sequence->ExecuteSingle(ui, __workspace.get());
}

ExecutionStatus Procedure::GetStatus() const
{
    return __root_sequence->GetStatus();
}
