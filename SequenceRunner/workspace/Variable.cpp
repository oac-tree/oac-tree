#include "Variable.h"


Variable::Variable(std::string name)
    : __name{std::move(name)}
    , __value{}
{}

Variable::Variable(const Variable &other) = default;

Variable::~Variable() = default;

std::string Variable::GetName() const
{
    return __name;
}

int Variable::GetValue() const
{
    return __value;
}

bool Variable::SetValue(int val)
{
    __value = val;
    return true;
}
