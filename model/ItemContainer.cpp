#include "ItemContainer.h"

namespace Sequencer {

namespace Model {

bool ItemContainer::ContainerFull() const
{
    return (__tag_info.GetMax() >= 0 && GetSize() >= __tag_info.GetMax());
}

bool ItemContainer::CanInsertItem(Item *item, int row) const
{
    return (__tag_info.IsValidItem(item->GetType())
            && !ContainerFull() && row <= GetSize());
}

ItemContainer::ItemContainer(TagInfo tag_info)
    : __tag_info(std::move(tag_info))
{}

ItemContainer::~ItemContainer()
{
    for (Item *item : __items)
    {
        delete item;
    }
}

ItemContainer::ItemContainer(ItemContainer && other)
{
    for (Item * item : other.__items)
    {
        __items.push_back(item);
    }
    other.__items.clear();
}

ItemContainer & ItemContainer::operator=(ItemContainer && other)
{
    for (Item * item : __items)
    {
        delete item;
    }
    for (Item * item : other.__items)
    {
        __items.push_back(item);
    }
    return *this;
}

int ItemContainer::GetSize() const
{
    return static_cast<int>(__items.size());
}

std::string ItemContainer::GetTag() const
{
    return __tag_info.Name();
}

TagInfo ItemContainer::GetTagInfo() const
{
    return __tag_info;
}

bool ItemContainer::InsertItem(Item * item, int row)
{
    if (!CanInsertItem(item, row))
    {
        return false;
    }
    if (row < 0)
    {
        __items.push_back(item);
    }
    else
    {
        __items.insert(__items.begin() + row, item);
    }
    return true;
}

Item * ItemContainer::GetItem(int row) const
{
    if (row < 0 || row >= GetSize())
    {
        return nullptr;
    }
    return __items[row];
}

Item * ItemContainer::RemoveItem(int row)
{
    if (row < 0 || row >= GetSize())
    {
        return nullptr;
    }
    auto it = __items.erase(__items.begin() + row);
    return *it;
}

std::vector<Item *> ItemContainer::GetAllItems() const
{
    return __items;
}



}  // namespace Model

}  // namespace Sequencer
