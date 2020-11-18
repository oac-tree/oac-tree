#include "Variable.h"


Variable::Variable(std::string name)
    : _name{std::move(name)}
    , _value{}
{}

Variable::Variable(const Variable &other) = default;

Variable::~Variable() = default;

std::string Variable::GetName() const
{
    return _name;
}

int Variable::GetValue() const
{
    return _value;
}

bool Variable::SetValue(int val)
{
    _value = val;
    return true;
}
