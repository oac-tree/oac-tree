#include "Item.h"

namespace Sequencer {

namespace Model {

bool Item::CompatibleChild(Item * /*item*/) const
{
    return true;
}

Item::Item(ItemType item_type)
    : __item_type(std::move(item_type))
{}

Item::~Item()
{
    for (auto child : __children)
        delete child;
}

ItemType Item::GetType() const
{
    return __item_type;
}

unsigned Item::GetChildrenCount() const
{
    return __children.size();
}

std::vector<Item *> Item::GetChildren() const
{
    return __children;
}

bool Item::AddChild(Item * item)
{
    if (CompatibleChild(item))
    {
        __children.push_back(item);
        return true;
    }
    return false;
}

}  // namespace Model

}  // namespace Sequencer

