#include "TagInfo.h"

#include <algorithm>

namespace Sequencer {

namespace Model {

TagInfo::TagInfo()
    : __name()
    , __min(0)
    , __max(-1)
    , __item_types()
{}

TagInfo::TagInfo(std::string name, int min, int max, std::vector<ItemType> item_types)
    : __name(std::move(name))
    , __min(min)
    , __max(max)
    , __item_types(std::move(item_types))
{}

TagInfo TagInfo::UniversalTag(std::string name, std::vector<ItemType> item_types)
{
    return TagInfo(std::move(name), 0, -1, std::move(item_types));
}

TagInfo TagInfo::PropertyTag(std::string name, ItemType item_type)
{
    return TagInfo(std::move(name), 1, 1, {std::move(item_type)});
}

std::string TagInfo::Name() const
{
    return __name;
}

int TagInfo::GetMin() const
{
    return __min;
}

int TagInfo::GetMax() const
{
    return __max;
}

std::vector<ItemType> TagInfo::GetItemTypes() const
{
    return __item_types;
}

bool TagInfo::IsValidItem(const ItemType &item_type) const
{
    if (__item_types.empty())
    {
        return true;
    }
    return std::find(__item_types.begin(), __item_types.end(), item_type) != __item_types.end();
}

bool TagInfo::IsPropertyTag() const
{
    return (__min==1 && __max==1 && __item_types.size()==1);
}




}  // namespace Model

}  // namespace Sequencer
