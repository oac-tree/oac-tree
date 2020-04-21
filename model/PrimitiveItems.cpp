#include "PrimitiveItems.h"

namespace Sequencer {

namespace Model {

bool StringItem::CompatibleChild(Item *) const
{
    return false;
}

StringItem::StringItem(std::string data)
    : Item(StringType)
    , __data(std::move(data))
{}

StringItem::~StringItem()
{}

void StringItem::SetData(std::string data)
{
    __data = std::move(data);
}

std::string StringItem::GetDisplayName() const
{
    return GetType();
}

std::string StringItem::GetData() const
{
    return __data;
}

}  // namespace Model

}
