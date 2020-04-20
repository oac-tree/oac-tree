#include "Item.h"

namespace Sequencer {

namespace Model {

Item::Item(ItemType item_type)
    : __item_type(std::move(item_type))
{}

Item::~Item() {}

ItemType Item::GetType() const
{
    return __item_type;
}

}  // namespace Model

}  // namespace Sequencer

