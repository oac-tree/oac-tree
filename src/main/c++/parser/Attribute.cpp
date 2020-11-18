#include "Attribute.h"

Attribute::Attribute(std::string name, std::string value)
  : _name{std::move(name)}
  , _value{std::move(value)}
{}

Attribute::~Attribute()
{}

const std::string Attribute::Name() const
{
    return _name;
}

const std::string Attribute::Value() const
{
    return _value;
}
