#include "Workspace.h"

#include <utility>

Workspace::Workspace()
    : _var_map{}
{}

Workspace::~Workspace() = default;

bool Workspace::AddVariable(Variable var)
{
    if (_var_map.find(var.GetName()) != _var_map.end())
    {
        return false;
    }
    _var_map.insert(std::map<std::string, Variable>::value_type(var.GetName(), var));
    return true;
}

std::vector<std::string> Workspace::VariableNames() const
{
    std::vector<std::string> result;
    for(const auto & entry : _var_map)
    {
        auto name = entry.first;
        result.push_back(name);
    }
    return result;
}

int Workspace::GetVariableValue(std::string name)
{
    auto it = _var_map.find(name);
    if (it == _var_map.end())
    {
        return 0;
    }
    auto var = it->second;
    return var.GetValue();
}

bool Workspace::SetVariableValue(std::string name, int value)
{
    auto it = _var_map.find(name);
    if (it == _var_map.end())
    {
        return false;
    }
    auto& var = it->second;
    var.SetValue(value);
    return true;
}
