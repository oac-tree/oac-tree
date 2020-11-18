#include "Procedure.h"

Procedure::Procedure()
    : _root_sequence{new Sequence()}
    , _workspace{}
{}

Procedure::~Procedure() = default;

bool Procedure::AddVariable(Variable var)
{
    return _workspace->AddVariable(var);
}

std::vector<std::string> Procedure::VariableNames() const
{
    return _workspace->VariableNames();
}

int Procedure::GetVariableValue(std::string name)
{
    return _workspace->GetVariableValue(name);
}

bool Procedure::SetVariableValue(std::string name, int value)
{
    return _workspace->SetVariableValue(name, value);
}

bool Procedure::PushInstruction(Instruction * instruction)
{
    _root_sequence->PushBack(instruction);
    return true;
}

void Procedure::ExecuteSingle(UserInterface * ui)
{
    _root_sequence->ExecuteSingle(ui, _workspace.get());
}

ExecutionStatus Procedure::GetStatus() const
{
    return _root_sequence->GetStatus();
}
