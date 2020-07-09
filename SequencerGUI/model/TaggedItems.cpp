#include "TaggedItems.h"

namespace Sequencer {

namespace Model {


TaggedItems::iterator TaggedItems::Container(const std::string & tag_name)
{
    auto pred = [tag_name](const ItemContainer & container) {
        return container.GetTag() == tag_name;
    };
    auto it_begin = __tagged_containers.begin();
    auto it_end = __tagged_containers.end();
    return std::find_if(it_begin, it_end, pred);
}

TaggedItems::const_iterator TaggedItems::Container(const std::string &tag_name) const
{
    auto pred = [tag_name](const ItemContainer & container) {
        return container.GetTag() == tag_name;
    };
    auto it_begin = __tagged_containers.begin();
    auto it_end = __tagged_containers.end();
    return std::find_if(it_begin, it_end, pred);
}

bool TaggedItems::RegisterTag(const TagInfo & tag_info)
{
    if (IsTag(tag_info.Name()))
    {
        return false;
    }
    __tagged_containers.emplace_back(tag_info);
    return true;
}

bool TaggedItems::IsTag(const std::string & tag_name) const
{
    return Container(tag_name) != __tagged_containers.end();
}

int TaggedItems::ItemCount(const std::string &tag_name) const
{
    if (!IsTag(tag_name))
    {
        return 0;
    }
    auto it = Container(tag_name);
    return it->GetSize();
}

int TaggedItems::TotalItemCount() const
{
    int result = 0;
    for (auto it = __tagged_containers.begin(); it != __tagged_containers.end(); ++it)
    {
        result += it->GetSize();
    }
    return result;
}

bool TaggedItems::InsertItem(Item *item, const std::string &tag_name, int row)
{
    if (!IsTag(tag_name))
    {
        return false;
    }
    auto it = Container(tag_name);
    return it->InsertItem(item, row);
}

Item * TaggedItems::RemoveItem(const std::string &tag_name, int row)
{
    if (!IsTag(tag_name))
    {
        return nullptr;
    }
    auto it = Container(tag_name);
    return it->RemoveItem(row);
}

bool TaggedItems::CanRemoveItem(const std::string &tag_name, int row) const
{
    if (!IsTag(tag_name))
    {
        return false;
    }
    auto it = Container(tag_name);
    return row >= 0 && row < it->GetSize();
}

Item * TaggedItems::GetItem(const std::string & tag_name, int row) const
{
    if (!IsTag(tag_name))
    {
        return nullptr;
    }
    auto it = Container(tag_name);
    return it->GetItem(row);
}

std::vector<Item *> TaggedItems::GetItems(const std::string & tag_name) const
{
    if (!IsTag(tag_name))
    {
        return {};
    }
    auto it = Container(tag_name);
    return it->GetAllItems();
}

std::vector<Item *> TaggedItems::GetAllItems() const
{
    std::vector<Item *> result;
    for (auto it = __tagged_containers.begin(); it != __tagged_containers.end(); ++it)
    {
        for (int i=0; i<it->GetSize(); ++i)
        {
            result.push_back(it->GetItem(i));
        }
    }
    return result;
}

bool TaggedItems::IsPropertyTag(const std::string &tag_name) const
{
    if (!IsTag(tag_name))
    {
        return false;
    }
    auto it = Container(tag_name);
    return it->GetTagInfo().IsPropertyTag();
}

}  // namespace Model

}  // namespace Sequencer
