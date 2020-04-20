#include "VariableItem.h"

namespace Sequencer {

namespace Model {

VariableItem::VariableItem(std::string name, Item *item)
    : Item(VariableType)
    , __var_name(std::move(name))
    , __value(item)
{}

VariableItem::~VariableItem() {}

const std::string VariableItem::GetVariableName() const
{
    return __var_name;
}

void VariableItem::SetVariableName(std::string name)
{
    __var_name = std::move(name);
}

const Item * VariableItem::GetItem() const
{
    return __value.get();
}

bool VariableItem::SetItem(Item * item)
{
    __value.reset(item);
    return true;
}

}  // namespace Model

}  // namespace Sequencer
